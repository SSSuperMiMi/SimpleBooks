//
//  PolygonArea.cpp
//  Simple Books
//
//  Created by Allen Zhao on 5/15/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "PolygonTouchResponder.h"
#include "RenderCenter.h"
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "global.h"
#import "Math.h"
#import "ios_sys.h"
#import "Math.h"
#import "MemoryPool.h"
#import "GraphicsProcessingCenter.h"
#import "GyroProcessingCenter.h"

unsigned PolygonTouchResponder::m_single_click_delay_ms_default = 500000;
void PolygonTouchResponder::SetDoubleClickTimeInterval(float DoubleClickTimeInterval)
{
    m_double_click_time_interval_microseconds = DoubleClickTimeInterval;
}

bool PolygonTouchResponder::LoadShader(const char* vpath, const char* fpath, unsigned* log)
{
    SAFE_DELETE(m_pShader);
    m_useShader = false;
    if (Shader::CreateShader(vpath, fpath, log, &m_pShader)) {
        return m_useShader = true;
    }

    switch (*log) {
        case shader_log_path_error:
            NSLog(@"Shader file path error.");
            
            break;
        case shader_log_failed_to_compile_vertex_shader:
            NSLog(@"Vertex shader compile error.");
            
            break;
        case shader_log_failed_to_compile_fragment_shader:
            NSLog(@"Fragment shader compile error.");
            
            break;
        case shader_log_failed_to_link_shader:
            NSLog(@"Shader link error.");
            
            break;
            
        default:
            break;
    }
    
    return false;
}

void PolygonTouchResponder::Move(vector_2d v)
{
    m_Position.x += v.dx;
    m_Position.y += v.dy;
}

void PolygonTouchResponder::EnableDoubleClick(bool b)
{
    m_enable_double_click = b;
}

bool PolygonTouchResponder::CreatePolygonTouchResponder(float* data, unsigned vertices_num, PolygonTouchResponder** pPolygonTouchResponder)
{
    if(data == nullptr || vertices_num < 3){*pPolygonTouchResponder = nullptr; return false;}
    return *pPolygonTouchResponder = new PolygonTouchResponder(data, vertices_num);
}

bool PolygonTouchResponder::RegisterCallback(pfn_callback pfn, unsigned type)
{
    if(pfn == nullptr)return false;
    
    switch (type) {
        case enum_single_finger_single_click:
            return m_pfn_single_finger_single_click = pfn;
            
        case enum_single_finger_double_click:
            return m_pfn_single_finger_double_click = pfn;
            
        case enum_two_finger_single_click:
            return m_pfn_two_finger_single_click = pfn;
            
        case enum_two_finger_double_click:
            return m_pfn_two_finger_double_click = pfn;
            
        case enum_long_press_began:
            return m_pfn_long_press_began = pfn;
            
        case enum_long_press_end:
            return m_pfn_long_press_end = pfn;
            
        case enum_pinch:
            return m_pfn_pinch = pfn;
        
        case enum_rotate:
            return m_pfn_rotate = pfn;
            
        case enum_pan:
            return m_pfn_pan = pfn;
            
        case enum_double_pan:
            return m_pfn_double_pan = pfn;
            
        case enum_ConstantFieldModel_end:
            return m_pfn_ConstantFieldModel_end = pfn;
            
        default:
            return false;
    }
}

void PolygonTouchResponder::DelayTwoFingerSingleClickAfterSeconds(void* args)
{
    if(args == nullptr)return;
    PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)args;
    
    if(pPolygon->m_current_event == enum_two_finger_single_click &&
       pPolygon->m_pfn_two_finger_single_click != nullptr)
    {
        pPolygon->m_pfn_two_finger_single_click(pPolygon);
    }
}

void PolygonTouchResponder::DelaySingleClickAfterSeconds(void* args)
{
    if(args == nullptr)return;
    PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)args;
    
    if(pPolygon->m_current_event == enum_single_finger_single_click &&
       pPolygon->m_pfn_single_finger_single_click != nullptr)
    {
        pPolygon->m_pfn_single_finger_single_click(pPolygon);
    }
}

void PolygonTouchResponder::EnableDirectOSTouchMsg(bool b)
{
    m_EnableDirectOSTouchMsg = b;
}

void PolygonTouchResponder::OSMove(point_2d p1, point_2d p2)
{
    vector_2d v;
    v.dx = p2.x - p1.x;
    v.dy = p2.y - p1.y;
    if(m_pParent != nullptr)
    {
        m_pParent->SetPosition(MakePoint2D(m_Position.x + v.dx, m_Position.y + v.dy));
    }
    else
    {
        SetPosition(MakePoint2D(m_Position.x + v.dx, m_Position.y + v.dy));
    }
    
    m_finger_touch_velocity.dx = m_finger_touch_velocity.dy = 0;
    m_LineVelocity.dx = m_LineVelocity.dy = 0;
}

bool PolygonTouchResponder::FindOSEvent(UITouch* event)
{
    int l = os_events.GetLength();
    for (int i = 0; i < l; ++i) {
        TouchSpeedSignPackage* package = (TouchSpeedSignPackage*)os_events.GetObjectAtIndex(i);
        if (package->event == event) {
            return true;
        }
    }
    
    return false;
}

TouchSpeedSignPackage* PolygonTouchResponder::FindTSSP(UITouch* obj)
{
    int l = os_events.GetLength();
    for (int i = 0; i < l; ++i) {
        TouchSpeedSignPackage* package = (TouchSpeedSignPackage*)os_events.GetObjectAtIndex(i);
        if (package->event == obj) {
            return package;
        }
    }
    
    return nullptr;
}

void PolygonTouchResponder::osTouchBegin(UITouch* event)
{
    TouchSpeedSignPackage* package = new TouchSpeedSignPackage();
    package->event = event;
    package->m_enable_increase_speed = true;
    package->time = 0;
    
    os_events.Push(package);
}

void PolygonTouchResponder::osTouchMoved(UITouch* event)
{
    TouchSpeedSignPackage* obj = FindTSSP(event);
    obj->time = ios_sys::GetUTCSysTime();
    
    CGPoint current_point=[event locationInView:[event view]];
    CGPoint previous_point=[event previousLocationInView:[event view]];
    
    point_2d gl_current_point = ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(current_point.x, current_point.y));
    point_2d gl_previous_point = ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(previous_point.x, previous_point.y));
    OSMove(gl_previous_point, gl_current_point);
}

void PolygonTouchResponder::DoWithFingerTouchVelocity(point_2d StartPoint)
{
    return;
}

void PolygonTouchResponder::osTouchEnded(UITouch* event)
{
    TouchSpeedSignPackage* obj = FindTSSP(event);
    CGPoint current_point=[event locationInView:[event view]];
    CGPoint previous_point=[event previousLocationInView:[event view]];
    
    point_2d gl_current_point = ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(current_point.x, current_point.y));
    point_2d gl_previous_point = ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(previous_point.x, previous_point.y));
    if (obj->m_enable_increase_speed == true) {
        double deltaMS = ios_sys::GetUTCSysTime() - obj->time;
        
        m_finger_touch_velocity.dx = (gl_current_point.x - gl_previous_point.x) / deltaMS;
        m_finger_touch_velocity.dy = (gl_current_point.y - gl_previous_point.y) / deltaMS;

        os_events.DeleteObject(obj);
        int l = os_events.GetLength();
        for (int i = 0; i < l; ++i) {
            TouchSpeedSignPackage* package = (TouchSpeedSignPackage*)os_events.GetObjectAtIndex(i);
            package->m_enable_increase_speed = false;
        }
        SAFE_DELETE(obj);
        DoWithFingerTouchVelocity(gl_previous_point);
        return;
    }
    os_events.DeleteObject(obj);
    SAFE_DELETE(obj);
}

void PolygonTouchResponder::OnTouchEvent(unsigned EventType)
{
    //
    // When a touch event occurs in the current dynamic polygon area, this function will be called automatically.
    //
    m_current_event = EventType;
    switch (EventType) {
        case enum_single_finger_single_click:
        {
            if(m_enable_double_click == false)
            {
                if(m_pfn_single_finger_single_click != nullptr)
                {
                    return m_pfn_single_finger_single_click(this);
                }
            }
            else
            {
                m_pSingleClickTimer = new CTimer(0, m_double_click_time_interval_microseconds, DelaySingleClickAfterSeconds, false, this);
                return m_pSingleClickTimer->StartTimer();
            }
            
            break;
        }
            
        case enum_single_finger_double_click:
        {
            if(m_enable_double_click == true && m_pfn_single_finger_double_click != nullptr)
            {
                return m_pfn_single_finger_double_click(this);
            }
            
            break;
        }
            
        case enum_two_finger_single_click:
        {
            if(m_enable_two_finger_double_click == false)
            {
                if(m_pfn_two_finger_single_click != nullptr)
                {
                    return m_pfn_two_finger_single_click(this);
                }
            }
            else
            {
                m_pTwoFingerSingleClickTimer = new CTimer(0, m_double_click_time_interval_microseconds, DelayTwoFingerSingleClickAfterSeconds, false, this);
                return m_pTwoFingerSingleClickTimer->StartTimer();
            }
            
            break;
        }
        
        case enum_two_finger_double_click:
        {
            if(m_enable_two_finger_double_click == true && m_pfn_two_finger_single_click != nullptr)
            {
                return m_pfn_two_finger_double_click(this);
            }
            
            break;
        }
            
        case enum_long_press_began:
        {
            if(m_pfn_long_press_began != nullptr)
            {
                return m_pfn_long_press_began(this);
            }
            
            break;
        }
            
        case enum_long_press_end:
        {
            if(m_pfn_long_press_end != nullptr)
            {
                return m_pfn_long_press_end(this);
            }
            
            break;
        }
            
        case enum_pinch:
        {
            if(m_pinch_recognizer != NULL &&
               m_pinch_recognizer.state == UIGestureRecognizerStateEnded)
            {
                m_pinching = false;
                m_pinch_recognizer = NULL;
                
                return;
            }
            
            if(m_pinching == true && m_pfn_pinch != NULL)
            {
                return m_pfn_pinch(this);
            }
            
            break;
        }
        
        case enum_rotate:
        {
            if(m_rotate_recognizer != NULL &&
               m_rotate_recognizer.state == UIGestureRecognizerStateEnded)
            {
                m_rotating = false;
                m_rotate_recognizer = NULL;
                
                return;
            }
            
            if(m_rotating == true && m_pfn_rotate != NULL)
            {
                return m_pfn_rotate(this);
            }
            
            break;
        }
            
        case enum_pan:
        {
            if(m_pan_recognizer != NULL &&
               m_pan_recognizer.state == UIGestureRecognizerStateEnded)
            {
                m_paning = false;
                m_pan_recognizer = NULL;
                
                return;
            }
            
            if(m_paning == true && m_pfn_pan != NULL)
            {
                return m_pfn_pan(this);
            }
            
            break;
        }
          
        case enum_double_pan:
        {
            if(m_double_pan_recognizer != NULL &&
               m_double_pan_recognizer.state == UIGestureRecognizerStateEnded)
            {
                m_double_paning = false;
                m_double_pan_recognizer = NULL;
                
                return;
            }
            
            if(m_double_paning == true && m_pfn_double_pan != NULL)
            {
                return m_pfn_double_pan(this);
            }
            
            break;
        }
        
        default:
            break;
    }
}

bool PolygonTouchResponder::IsPointWithinThisArea(point_2d pTargetPoint)
{
    bool isInvertible = true;
    GLKMatrix4 m_modelViewMatrix_invert = GLKMatrix4Invert(m_modelViewMatrix, &isInvertible);
    CGPoint pTargetPoint_invert = CGPointMake(
                                              pTargetPoint.x * m_modelViewMatrix_invert.m00 +
                                              pTargetPoint.y * m_modelViewMatrix_invert.m10 +
                                              1.0f * m_modelViewMatrix_invert.m30,
                                              pTargetPoint.x * m_modelViewMatrix_invert.m01 +
                                              pTargetPoint.y * m_modelViewMatrix_invert.m11 +
                                              1.0f * m_modelViewMatrix_invert.m31
                                              );
    if ([m_ClosePath containsPoint:pTargetPoint_invert]) {
        memcpy(&m_CurrentSingleClickPoint, &pTargetPoint, sizeof(point_2d));
        return true;
    }
    
    return false;
}

void PolygonTouchResponder::ResetRealTouchArea(float touch_area_magnification_factor)
{
    if(touch_area_magnification_factor < 1.0f)return;
    m_touch_area_magnification_factor = touch_area_magnification_factor;
    SetRealTouchArea();
}

void PolygonTouchResponder::SetRealTouchArea()
{
    global_memory_pool.ReleaseMemory(m_real_touch_area_vertices);
    global_memory_pool.GetMemory(m_vertices_num * sizeof(CGPoint), (void**)&m_real_touch_area_vertices);

    for(int i = 0; i < m_vertices_num; ++i)
    {
        m_real_touch_area_vertices[i].x = m_vertices_2d[i].x * m_touch_area_magnification_factor;
        m_real_touch_area_vertices[i].y = m_vertices_2d[i].y * m_touch_area_magnification_factor;
    }
}

void PolygonTouchResponder::PrintGLDataLog(float* data)
{
    for (int i = 0; i < m_vertices_num; ++i) {
        NSLog(@"顶点%d : {%g, %g, %g, %g, %g, %g, %g}",
              i,
              data[i * 7],
              data[i * 7 + 1],
              data[i * 7 + 2],
              data[i * 7 + 3],
              data[i * 7 + 4],
              data[i * 7 + 5],
              data[i * 7 + 6]
              );
    }
}

void PolygonTouchResponder::PrintGLDataLog()
{
    for (int i = 0; i < m_vertices_num; ++i) {
        NSLog(@"顶点%d : {%g, %g, %g, %g, %g, %g, %g}",
              i,
              m_gl_data[i * 7],
              m_gl_data[i * 7 + 1],
              m_gl_data[i * 7 + 2],
              m_gl_data[i * 7 + 3],
              m_gl_data[i * 7 + 4],
              m_gl_data[i * 7 + 5],
              m_gl_data[i * 7 + 6]
              );
    }
}

GLKMatrix4 PolygonTouchResponder::GetModelViewMatrix()
{
    return m_modelViewMatrix;
}

void PolygonTouchResponder::Transform()
{
    math::GLKMatrix4LoadIdentity(&m_modelViewMatrix);

    m_modelViewMatrix = GLKMatrix4Translate(
                                            m_modelViewMatrix,
                                            m_Position.x,
                                            m_Position.y,
                                            0);
    m_modelViewMatrix = GLKMatrix4Rotate(
                                         m_modelViewMatrix,
                                         m_angle, 0.0, 0.0, 1.0);
    
    m_modelViewMatrix = GLKMatrix4Scale(m_modelViewMatrix, m_scale_x, m_scale_y, 1);
    m_projectionMatrix = GLKMatrix4MakeScale(1.0f, AspectRatio, 1.0f);
    m_MVP = GLKMatrix4Multiply(m_projectionMatrix, m_modelViewMatrix);
}

point_2d PolygonTouchResponder::GetPosition()
{
    return  m_Position;
}

vector_2d PolygonTouchResponder::GetScale()
{
    return MakeVector2d(m_scale_x, m_scale_y);
}

float  PolygonTouchResponder::GetRotateAngle()
{
    return m_angle;
}

void PolygonTouchResponder::Rotate(float radian)
{
    m_angle += radian;
    Transform();
}

void PolygonTouchResponder::SetScale(float factor_x, float factor_y)
{
    m_scale_x = factor_x;
    m_scale_y = factor_y;
    Transform();
}

void PolygonTouchResponder::SetPosition(point_2d position)
{
    memcpy(&m_Position, &position, sizeof(point_2d));
    Transform();
}

void PolygonTouchResponder::Update()
{
    double m_time_now = ios_sys::GetUTCSysTime();
    if (m_time_last_frame == 0) {
        m_time_last_frame = m_time_now;
    }
    deltaMS = m_time_now - m_time_last_frame;
    m_time_last_frame = m_time_now;
}

void PolygonTouchResponder::SetVisible(bool IsVisible)
{
    if(m_isRendering == false)return;

    if (IsVisible == true) {
        AttachEvent();
    }
    else
    {
        DetachEvent();
    }
    m_isVisible = IsVisible;
}

void PolygonTouchResponder::SetRenderMode(unsigned RenderMode)
{
    m_RenderMode = RenderMode;
}

void PolygonTouchResponder::PrepareData()
{
    Update();
    PrepareModel();
    PreparePosition();
    Transform();
    if(m_useShader == false)
    {
        m_positionSlot = GLKVertexAttribPosition;
        m_colorSlot = GLKVertexAttribColor;
        
        m_pBaseEffect.transform.modelviewMatrix = m_modelViewMatrix;
        m_pBaseEffect.transform.projectionMatrix = m_projectionMatrix;
        
        [m_pBaseEffect prepareToDraw];
    }
}

void PolygonTouchResponder::UpdateShaderValues()
{
    m_pShader->UpdateUniformValue("MVP", 1, 0, (GLfloat*)&m_MVP, 4, 4);
    m_pShader->UpdateUniformValue("UColor", 1, 4, s_color);
}

void PolygonTouchResponder::Rendering()
{
    if (!m_isVisible || !m_isRendering)return;
    if (m_useShader == TRUE) {
        m_positionSlot = m_pShader->GetAttributeLocation("Position");
        m_colorSlot    = m_pShader->GetAttributeLocation("Color");
    }
   
    glEnableVertexAttribArray(m_positionSlot);
    glEnableVertexAttribArray(m_colorSlot);
    glBindBuffer(GL_ARRAY_BUFFER,m_vao_id);
    
    if (m_useShader == TRUE) {
        m_pShader->Shading();
        UpdateShaderValues();
    }
    
    glVertexAttribPointer(
                          m_positionSlot,
                          vao_vertex_components_num,
                          GL_FLOAT,
                          GL_FALSE,
                          vao_attribute_gap,
                          vao_vertex_offset
                          );
    glVertexAttribPointer(
                          m_colorSlot,
                          vao_color_components_num,
                          GL_FLOAT,
                          GL_FALSE,
                          vao_attribute_gap,
                          vao_color_offset
                          );
    glDrawArrays(m_RenderMode,
                 0,
                 m_vertices_num);
}

void PolygonTouchResponder::SetCenterVectors()
{
    global_memory_pool.GetMemory(m_vertices_num * sizeof(vector_2d), (void**)&m_center_vectors);
    
    for(int i = 0; i < m_vertices_num; ++i)
    {
        m_center_vectors[i].dx = m_vertices_2d[i].x;
        m_center_vectors[i].dy = m_vertices_2d[i].y;
    }
}

void PolygonTouchResponder::AttachRendering()
{
    render.Add(this);
    m_isRendering = true;
}

void PolygonTouchResponder::DetachRendering()
{
    render.Remove(this);
    m_isRendering = false;
}

void PolygonTouchResponder::AttachEvent()
{
    MsgCenter.AddEventListener(this);
}

void PolygonTouchResponder::DetachEvent()
{
    MsgCenter.RemoveEventListener(this);
}

void PolygonTouchResponder::EnableTwoFingerDoubleClick(bool b)
{
    m_enable_two_finger_double_click = b;
}

void PolygonTouchResponder::ModelCopy(PolygonTouchResponder** pPolygonTouchResponder)
{
    *pPolygonTouchResponder = new PolygonTouchResponder(m_gl_data, m_vertices_num);
}

PolygonTouchResponder::PolygonTouchResponder(float* data, unsigned vertices_num):
        m_center_vectors(nullptr),
        m_vertices_2d(nullptr),
        m_real_touch_area_vertices(nullptr),
        m_pfn_single_finger_single_click(nullptr),
        m_pfn_single_finger_double_click(nullptr),
        m_pfn_two_finger_single_click(nullptr),
        m_pfn_two_finger_double_click(nullptr),
        m_pfn_long_press_began(nullptr),
        m_pfn_long_press_end(nullptr),
        m_pfn_rotate(NULL),
        m_enable_double_click(false),
        m_enable_two_finger_double_click(false),
        m_double_click_time_interval_microseconds(m_single_click_delay_ms_default),
        m_current_event(enum_no_touch_event),
        m_pSingleClickTimer(NULL),
        m_pTwoFingerSingleClickTimer(NULL),
        m_ClosePath(nullptr),
        m_pinching(false),
        m_rotating(false),
        m_pinch_recognizer(NULL),
        m_rotate_recognizer(NULL),
        m_paning(false),
        m_pan_recognizer(NULL),
        m_pfn_pan(NULL),
        m_double_paning(NULL),
        m_double_pan_recognizer(NULL),
        m_pfn_double_pan(NULL),
        m_vao_id(0),
        m_gl_data(0),
        m_scale_x(1.0f),
        m_scale_y(1.0f),
        m_angle(0),
        m_isRendering(false),
        m_isVisible(true),
        m_RenderMode(GL_TRIANGLE_FAN),
        m_pShader(nullptr),
        m_useShader(false),
        m_pBaseEffect(nullptr),
        m_positionSlot(0),
        m_colorSlot(0),
        m_AngleAcceleration(0.0f),
        m_AngleVelocity(0.0f),
        pUniform_speed_deform_args(NULL),
        ParabolaTracking(nullptr),
        m_pParent(nullptr),
        m_time_last_frame(0),
        deltaMS(0),
        m_gl_preliminary_data(0),
        m_pfn_ConstantFieldModel_end(nullptr),
        m_EnableDirectOSTouchMsg(false)
{
    m_touch_area_magnification_factor = touch_area_magnification_factor_default;
    unsigned mm_length = vertices_num * sizeof(point_2d);
    global_memory_pool.GetMemory(mm_length, (void**)&m_vertices_2d);
    for (int i = 0; i < vertices_num; ++i) {
        memcpy(&m_vertices_2d[i], &data[i * 7], sizeof(point_2d));
    }
    m_vertices_num = vertices_num;
    m_center_point.x = 0.0f;
    m_center_point.y = 0.0f;
    
    float area = 0.0f;
    float p1X  = 0.0f;
    float p1Y  = 0.0f;
    float inv3 = 1.0f/3.0f;
    for(int i = 0; i < m_vertices_num; ++i) {
        point_2d p2 = m_vertices_2d[i];
        point_2d p3 = (i+1)<m_vertices_num ? m_vertices_2d[i+1] : m_vertices_2d[0];
        
        float e1X = p2.x - p1X;
        float e1Y = p2.y - p1Y;
        float e2X = p3.x - p1X;
        float e2Y = p3.y - p1Y;
        float D = e1X * e2Y - e1Y * e2X;
        float triangleArea = 0.5f * D;
        
        area += triangleArea;
        
        m_center_point.x += triangleArea * inv3 * (p1X + p2.x + p3.x);
        m_center_point.y += triangleArea * inv3 * (p1Y + p2.y + p3.y);
    }
    
    if(area > FLT_EPSILON) {
        m_center_point.x *=1.0f/area;
        m_center_point.y *=1.0f/area;
    } else {
        m_center_point.x = 0xffffffff;
        m_center_point.y = 0xffffffff;
    }
    global_memory_pool.GetMemory(vertices_num * 7 * sizeof(float), (void**)&m_gl_data);
    memset(m_gl_data, 0, vertices_num * 7 * sizeof(float));
    
    for (int i = 0; i < m_vertices_num; ++i) {
        m_gl_data[i * 7] = data[i * 7] - m_center_point.x;
        m_gl_data[i * 7 + 1] = data[i * 7 + 1] - m_center_point.y;
        for (int j = 2; j < 7; ++j) {
            m_gl_data[i * 7 + j] = data[i * 7 + j];
        }
    }
    
    global_memory_pool.ReleaseMemory(m_vertices_2d);
    global_memory_pool.GetMemory(mm_length, (void**)&m_vertices_2d);
    for (int i = 0; i < vertices_num; ++i) {
        memcpy(&m_vertices_2d[i], &m_gl_data[i * 7], sizeof(point_2d));
    }
    
    memset(&m_center_point, 0, sizeof(point_2d));
    memset(&m_Position, 0, sizeof(point_2d));
    memset(&GyroscopeSpeedBonus, 0, sizeof(vector_2d));
    
    SetCenterVectors();
    SetRealTouchArea();
    
    m_ClosePath = [[UIBezierPath alloc] init];
    [m_ClosePath moveToPoint:m_real_touch_area_vertices[0]];
    for(int i = 1; i < m_vertices_num; ++i)
    {
        [m_ClosePath addLineToPoint:m_real_touch_area_vertices[i]];
    }
    
    math::GLKMatrix4LoadIdentity(&m_modelViewMatrix);
    math::GLKMatrix4LoadIdentity(&m_projectionMatrix);
    math::GLKMatrix4LoadIdentity(&m_MVP);
    
    glGenBuffers(1, &m_vao_id);
    glBindBuffer(GL_ARRAY_BUFFER,
                 m_vao_id);
    glBufferData(
                 GL_ARRAY_BUFFER,
                 vertices_num * 7 * sizeof(float),
                 m_gl_data,
                 GL_STATIC_DRAW);
    
    Transform();
    
    AttachEvent();
    AttachRendering();
    
    m_LineAcceleration.dx = m_LineAcceleration.dy = m_LineVelocity.dx = m_LineVelocity.dy = 0.0f;
    global_memory_pool.GetMemory(vertices_num * 7 * sizeof(float), (void**)&m_gl_preliminary_data);
    
    memset(s_color, 0, 4 * sizeof(float));
    memcpy(m_gl_preliminary_data, m_gl_data, vertices_num * 7 * sizeof(float));
    memset(&m_GyrovBonus, 0, sizeof(vector_2d));
    memset(&m_Scale_Velocity, 0, sizeof(vector_2d));
    memset(&M_Scale_Acceleration, 0, sizeof(vector_2d));
    memset(&m_finger_touch_velocity, 0, sizeof(vector_2d));
    memset(&m_CurrentSingleClickPoint, 0, sizeof(point_2d));
}

void PolygonTouchResponder::Dock()
{
    DetachEvent();
    DetachGyroPC();
}

void PolygonTouchResponder::UnDock()
{
    AttachEvent();
    AttachGyroPC();
}

void PolygonTouchResponder::AttachGyroPC()
{
    _GyroPC_.Add(this);
}

void PolygonTouchResponder::DetachGyroPC()
{
    _GyroPC_.Remove(this);
}

void PolygonTouchResponder::GetColor(float** color)
{
    memcpy(*color, s_color, 4 * sizeof(float));
}

bool PolygonTouchResponder::SetColor(float* color4)
{
    if (color4 == nullptr) {
        return false;
    }
    
    memcpy(s_color, color4, 4 * sizeof(float));
    return true;
}

void PolygonTouchResponder::SetLineAcceleration(vector_2d a)
{
    memcpy(&m_LineAcceleration, &a, sizeof(vector_2d));
}

void PolygonTouchResponder::SetLineVelocity(vector_2d v)
{
    memcpy(&m_LineVelocity, &v, sizeof(vector_2d));
}

void PolygonTouchResponder::SetAngleVelocity(float s)
{
    m_AngleVelocity = s;
}

void PolygonTouchResponder::SetAngleAcceleration(float s)
{
    m_AngleAcceleration = s;
}

bool PolygonTouchResponder::PolygonDeforming(uniform_speed_deform_args* ptr)
{
    if (ptr == NULL || ptr->m_Frames == 0) {
        return false;
    }
    
    if ((ptr->m_TargetAngleAcceleration * ptr->m_TargetAngleVelocity) != 0) {
        return false;
    }
    
    PolygonTouchResponder* p = ptr->m_pTarget;
    if (
        p == nullptr ||
        m_LineAcceleration.dx != 0 ||
        m_LineAcceleration.dy != 0 ||
        m_LineVelocity.dx != 0 ||
        m_LineVelocity.dy != 0 ||
        p->m_LineAcceleration.dx != 0 ||
        p->m_LineAcceleration.dy != 0 ||
        p->m_LineVelocity.dx != 0 ||
        p->m_LineVelocity.dy != 0 ||
        pUniform_speed_deform_args != nullptr
        )
    {
        return false;
    }
    
    global_memory_pool.GetMemory(sizeof(uniform_speed_deform_args), (void**)&pUniform_speed_deform_args);
    memcpy(pUniform_speed_deform_args, ptr, sizeof(uniform_speed_deform_args));
    if (p == this) {
        pUniform_speed_deform_args->m_self_transform = true;
    }
    
    return true;
}

bool PolygonTouchResponder::PolygonDeforming(ConstantFieldModel* ptr)
{
    if (ptr == NULL || ptr->m_Frames == 0.0f) {
        return false;
    }
    
    if (ptr->m_pTarget != nullptr) {
        if ((ptr->m_TargetAngleAcceleration * ptr->m_TargetAngleVelocity) != 0) {
            return false;
        }
        
        PolygonTouchResponder* p = ptr->m_pTarget;
        if (
            p == nullptr ||
            m_LineAcceleration.dx != 0 ||
            m_LineAcceleration.dy != 0 ||
            m_LineVelocity.dx != 0 ||
            m_LineVelocity.dy != 0 ||
            p->m_LineAcceleration.dx != 0 ||
            p->m_LineAcceleration.dy != 0 ||
            p->m_LineVelocity.dx != 0 ||
            p->m_LineVelocity.dy != 0 ||
            pUniform_speed_deform_args != nullptr
            )
        {
            return false;
        }
        
        if (p->m_RenderMode != m_RenderMode) {
            return false;
        }
    }
    
    global_memory_pool.GetMemory(sizeof(ConstantFieldModel), (void**)&ParabolaTracking);
    memcpy(ParabolaTracking, ptr, sizeof(ConstantFieldModel));
    if (ptr->m_pTarget == this) {
        ParabolaTracking->m_self_transform = true;
    }
    return true;
}

void PolygonTouchResponder::ToBeStatic()
{
    m_AngleAcceleration = m_AngleVelocity = 0;
    m_LineAcceleration.dx = m_LineAcceleration.dy = 0;
    m_LineVelocity.dx = m_LineVelocity.dy = 0;
    M_Scale_Acceleration.dx = M_Scale_Acceleration.dy = 0;
    m_Scale_Velocity.dx = m_Scale_Velocity.dy = 0;
}

void PolygonTouchResponder::PrepareModel()
{
    //
    // 此函数仅在不使用shader下成立
    //
    if (pUniform_speed_deform_args == nullptr && ParabolaTracking == nullptr){
        return;
    }

    if(pUniform_speed_deform_args != nullptr)
    {
        PolygonTouchResponder* Target = pUniform_speed_deform_args->m_pTarget;
        if (pUniform_speed_deform_args->m_DeformingKeyframe == 0 && pUniform_speed_deform_args->m_self_transform == false) {
            unsigned VertexDifferenceCount = math::uDifferenceAbs(m_vertices_num, Target->m_vertices_num);
            if (VertexDifferenceCount != 0) {
                if (m_vertices_num > Target->m_vertices_num) {
                    Target->VertexCompensation(VertexDifferenceCount);
                }
                else
                {
                    VertexCompensation(VertexDifferenceCount);
                }
            }
            pUniform_speed_deform_args->SetBeginGL_data(m_gl_data);
        }
        ++pUniform_speed_deform_args->m_DeformingKeyframe;
        pUniform_speed_deform_args->m_MS += deltaMS;
        
        SetScale(pUniform_speed_deform_args->GetSize().dx, pUniform_speed_deform_args->GetSize().dy);

        if (pUniform_speed_deform_args->m_self_transform == false) {
            float DeformationRate = pUniform_speed_deform_args->m_MS / pUniform_speed_deform_args->m_Time;
            float* new_gl_data;
            float* gl_data_before_deform = pUniform_speed_deform_args->m_begin_gl_data;
            global_memory_pool.GetMemory(m_vertices_num * 7 * sizeof(float), (void**)&new_gl_data);
            memcpy(new_gl_data, gl_data_before_deform, m_vertices_num * 7 * sizeof(float));
            
            for (int i = 0; i < m_vertices_num; ++i)
            {
                new_gl_data[i * 7 + 0] += (Target->m_gl_data[i * 7 + 0] - gl_data_before_deform[i * 7 + 0]) * DeformationRate;
                new_gl_data[i * 7 + 1] += (Target->m_gl_data[i * 7 + 1] - gl_data_before_deform[i * 7 + 1]) * DeformationRate;
                new_gl_data[i * 7 + 2] += (Target->m_gl_data[i * 7 + 2] - gl_data_before_deform[i * 7 + 2]) * DeformationRate;
                new_gl_data[i * 7 + 3] += (Target->m_gl_data[i * 7 + 3] - gl_data_before_deform[i * 7 + 3]) * DeformationRate;
                new_gl_data[i * 7 + 4] += (Target->m_gl_data[i * 7 + 4] - gl_data_before_deform[i * 7 + 4]) * DeformationRate;
                new_gl_data[i * 7 + 5] += (Target->m_gl_data[i * 7 + 5] - gl_data_before_deform[i * 7 + 5]) * DeformationRate;
                new_gl_data[i * 7 + 6] += (Target->m_gl_data[i * 7 + 6] - gl_data_before_deform[i * 7 + 6]) * DeformationRate;
            }
            ModelRemap(new_gl_data, m_vertices_num);
            global_memory_pool.ReleaseMemory(new_gl_data);
        }
    }
    
    if (ParabolaTracking != nullptr) {
        PolygonTouchResponder* Target = ParabolaTracking->m_pTarget;
         if (ParabolaTracking->m_DeformingKeyframe == 0 && ParabolaTracking->m_self_transform == false)
         {
             unsigned VertexDifferenceCount = math::uDifferenceAbs(m_vertices_num, Target->m_vertices_num);
             if (VertexDifferenceCount != 0) {
                 if (m_vertices_num > Target->m_vertices_num) {
                     Target->VertexCompensation(VertexDifferenceCount);
                 }
                 else
                 {
                     VertexCompensation(VertexDifferenceCount);
                 }
             }
             ParabolaTracking->SetBeginGL_data(m_gl_data);
         }
        ++ParabolaTracking->m_DeformingKeyframe;
        ParabolaTracking->m_MS += deltaMS;
        SetScale(ParabolaTracking->GetSize().dx, ParabolaTracking->GetSize().dy);
        if (ParabolaTracking->m_self_transform == false) {
            float DeformationRate = ParabolaTracking->m_MS / ParabolaTracking->m_Time;
                float* new_gl_data;
                float* gl_data_before_deform = ParabolaTracking->m_begin_gl_data;
                global_memory_pool.GetMemory(m_vertices_num * 7 * sizeof(float), (void**)&new_gl_data);
                memcpy(new_gl_data, gl_data_before_deform, m_vertices_num * 7 * sizeof(float));
                
                for (int i = 0; i < m_vertices_num; ++i)
                {
                    new_gl_data[i * 7 + 0] += (Target->m_gl_data[i * 7 + 0] - gl_data_before_deform[i * 7 + 0]) * DeformationRate;
                    new_gl_data[i * 7 + 1] += (Target->m_gl_data[i * 7 + 1] - gl_data_before_deform[i * 7 + 1]) * DeformationRate;
                    new_gl_data[i * 7 + 2] += (Target->m_gl_data[i * 7 + 2] - gl_data_before_deform[i * 7 + 2]) * DeformationRate;
                    new_gl_data[i * 7 + 3] += (Target->m_gl_data[i * 7 + 3] - gl_data_before_deform[i * 7 + 3]) * DeformationRate;
                    new_gl_data[i * 7 + 4] += (Target->m_gl_data[i * 7 + 4] - gl_data_before_deform[i * 7 + 4]) * DeformationRate;
                    new_gl_data[i * 7 + 5] += (Target->m_gl_data[i * 7 + 5] - gl_data_before_deform[i * 7 + 5]) * DeformationRate;
                    new_gl_data[i * 7 + 6] += (Target->m_gl_data[i * 7 + 6] - gl_data_before_deform[i * 7 + 6]) * DeformationRate;
                }
                ModelRemap(new_gl_data, m_vertices_num);
                global_memory_pool.ReleaseMemory(new_gl_data);
        }
    }
}

void PolygonTouchResponder::PreparePosition()
{
    m_LineVelocity.dx += deltaMS * m_LineAcceleration.dx;
    m_LineVelocity.dy += deltaMS * m_LineAcceleration.dy;
    
    m_Scale_Velocity.dx += deltaMS * M_Scale_Acceleration.dx;
    m_Scale_Velocity.dy += deltaMS * M_Scale_Acceleration.dy;
    
    m_AngleVelocity += deltaMS * m_AngleAcceleration;

    m_Position.x += deltaMS * m_LineVelocity.dx;
    m_Position.y += deltaMS * m_LineVelocity.dy;
    
    m_scale_x += m_Scale_Velocity.dx;
    m_scale_y += m_Scale_Velocity.dy;
    
    m_angle += deltaMS * m_AngleVelocity;
    
    if (pUniform_speed_deform_args == NULL && ParabolaTracking == nullptr) {
        return;
    }

    if (pUniform_speed_deform_args != nullptr) {
        SetPosition(pUniform_speed_deform_args->GetPos());
        if (pUniform_speed_deform_args->m_AngleVelocity_Interpolation) {
            m_AngleVelocity = pUniform_speed_deform_args->GetAngleVelocity();
        }
        else
        {
            m_AngleAcceleration = pUniform_speed_deform_args->GetAngleAcceleration();
        }
        
        if (pUniform_speed_deform_args->m_DeformingKeyframe == pUniform_speed_deform_args->m_Frames)
        {
            pUniform_speed_deform_args->Reset();
            pUniform_speed_deform_args = nullptr;
            global_memory_pool.ReleaseMemory(pUniform_speed_deform_args);
        }
    }
    
    if (ParabolaTracking != nullptr) {
        SetPosition(ParabolaTracking->GetPos());
        if (ParabolaTracking->m_AngleVelocity_Interpolation) {
            m_AngleVelocity = ParabolaTracking->GetAngleVelocity();
        }
        else
        {
            m_AngleAcceleration = ParabolaTracking->GetAngleAcceleration();
        }
        if (ParabolaTracking->m_DeformingKeyframe == ParabolaTracking->m_Frames)
        {
            ParabolaTracking->Reset();
            ParabolaTracking = nullptr;
            global_memory_pool.ReleaseMemory(ParabolaTracking);
            if(m_pfn_ConstantFieldModel_end != nullptr)
            {
                m_pfn_ConstantFieldModel_end(this);
            }
        }
    }
}

void PolygonTouchResponder::ModelRemap(float* data, unsigned vertices_num)
{
    if (data == nullptr || vertices_num == 0) {
        return;
    }
    
    global_memory_pool.ReleaseMemory(m_vertices_2d);
    unsigned mm_length = vertices_num * sizeof(point_2d);
    global_memory_pool.GetMemory(mm_length, (void**)&m_vertices_2d);
    for (int i = 0; i < vertices_num; ++i) {
        memcpy(&m_vertices_2d[i], &data[i * 7], sizeof(point_2d));
    }
    m_vertices_num = vertices_num;
    
    m_center_point.x = 0.0f;
    m_center_point.y = 0.0f;
    
    float area = 0.0f;
    float p1X  = 0.0f;
    float p1Y  = 0.0f;
    float inv3 = 1.0f/3.0f;
    
    for(int i = 0; i < m_vertices_num; ++i) {
        point_2d p2 = m_vertices_2d[i];
        point_2d p3 = (i+1)<m_vertices_num ? m_vertices_2d[i+1] : m_vertices_2d[0];
        
        float e1X = p2.x - p1X;
        float e1Y = p2.y - p1Y;
        float e2X = p3.x - p1X;
        float e2Y = p3.y - p1Y;
        float D = e1X * e2Y - e1Y * e2X;
        float triangleArea = 0.5f * D;
        
        area += triangleArea;
        
        m_center_point.x += triangleArea * inv3 * (p1X + p2.x + p3.x);
        m_center_point.y += triangleArea * inv3 * (p1Y + p2.y + p3.y);
    }
    
    if(area > FLT_EPSILON) {
        m_center_point.x *=1.0f/area;
        m_center_point.y *=1.0f/area;
    } else {
        m_center_point.x = 0xffffffff;
        m_center_point.y = 0xffffffff;
    }
    global_memory_pool.ReleaseMemory(m_gl_data);
    global_memory_pool.GetMemory(vertices_num * 7 * sizeof(float), (void**)&m_gl_data);
    memset(m_gl_data, 0, vertices_num * 7 * sizeof(float));
    
    for (int i = 0; i < m_vertices_num; ++i) {
        m_gl_data[i * 7] = data[i * 7] - m_center_point.x;
        m_gl_data[i * 7 + 1] = data[i * 7 + 1] - m_center_point.y;
        for (int j = 2; j < 7; ++j) {
            m_gl_data[i * 7 + j] = data[i * 7 + j];
        }
    }
    
    global_memory_pool.ReleaseMemory(m_vertices_2d);
    global_memory_pool.GetMemory(mm_length, (void**)&m_vertices_2d);
    for (int i = 0; i < vertices_num; ++i) {
        memcpy(&m_vertices_2d[i], &m_gl_data[i * 7], sizeof(point_2d));
    }
    
    memset(&m_center_point, 0, sizeof(point_2d));
    
    SetCenterVectors();
    SetRealTouchArea();
    
    [m_ClosePath removeAllPoints];
    [m_ClosePath moveToPoint:m_real_touch_area_vertices[0]];
    for(int i = 1; i < m_vertices_num; ++i)
    {
        [m_ClosePath addLineToPoint:m_real_touch_area_vertices[i]];
    }
    
    glDeleteBuffers(1, &m_vao_id);
    glGenBuffers(1, &m_vao_id);
    glBindBuffer(GL_ARRAY_BUFFER,
                 m_vao_id);
    glBufferData(
                 GL_ARRAY_BUFFER,
                 vertices_num * 7 * sizeof(float),
                 m_gl_data,
                 GL_STATIC_DRAW);
}

void PolygonTouchResponder::EnableShader(bool b)
{
    if (m_pShader == nullptr) {
            return;
    }
    m_useShader = b;
}

void PolygonTouchResponder::InsertCompensationVertex(float* data, unsigned vertex_num)
{
    float max_side_length_pow_2 = 0;
    
    float _x = 0.0f;
    float _y = 0.0f;
    float _z = 0.0f;
    float _r = 0.0f;
    float _g = 0.0f;
    float _b = 0.0f;
    float _a = 0.0f;
    
    unsigned mm_offset = 0;
    float* ptr = nullptr;
    for (int i = 0; i < vertex_num - 1; ++i) {
        float v1x = data[i * 7 + 0];
        float v1y = data[i * 7 + 1];
        float v1z = data[i * 7 + 2];
        float v1r = data[i * 7 + 3];
        float v1g = data[i * 7 + 4];
        float v1b = data[i * 7 + 5];
        float v1a = data[i * 7 + 6];
        
        float v2x = data[i * 7 + 7];
        float v2y = data[i * 7 + 8];
        float v2z = data[i * 7 + 9];
        float v2r = data[i * 7 + 10];
        float v2g = data[i * 7 + 11];
        float v2b = data[i * 7 + 12];
        float v2a = data[i * 7 + 13];
        
        float side_length_pow_2 = powf((v2x - v1x), 2) + powf((v2y - v1y), 2);
        if (side_length_pow_2 > max_side_length_pow_2) {
            max_side_length_pow_2 = side_length_pow_2;
            mm_offset = (vertex_num - i - 1) * 7 * sizeof(float);
            ptr = &data[i * 7 + 7];
            _x = (v1x + v2x) / 2;
            _y = (v1y + v2y) / 2;
            _z = (v1z + v2z) / 2;
            _r = (v1r + v2r) / 2;
            _g = (v1g + v2g) / 2;
            _b = (v1b + v2b) / 2;
            _a = (v1a + v2a) / 2;
        }
    }
    
    memmove(ptr + 7, ptr, mm_offset);
    
    ptr[0] = _x;
    ptr[1] = _y;
    ptr[2] = _z;
    ptr[3] = _r;
    ptr[4] = _g;
    ptr[5] = _b;
    ptr[6] = _a;
}

void PolygonTouchResponder::VertexCompensation(unsigned Factor)
{
    if(Factor == 0)return;
    unsigned total_vertex_num = m_vertices_num + Factor;
    unsigned mm_length = total_vertex_num * 7 * sizeof(float);
    float* data;
    global_memory_pool.GetMemory(mm_length, (void**)&data);
    memset(data, 0, mm_length);
    memcpy(data, m_gl_data, m_vertices_num * 7 * sizeof(float));
    
    for (int i = 0; i < Factor; ++i) {
        InsertCompensationVertex(data, m_vertices_num + i);
    }
    ModelRemap(data, total_vertex_num);
    global_memory_pool.ReleaseMemory(data);
}

void PolygonTouchResponder::OriginalStaticDeform(float* v_scale_factors, unsigned vertex_num)
{
    if (vertex_num != m_vertices_num || v_scale_factors == nullptr) {
        return;
    }
    float* data;
    global_memory_pool.GetMemory(m_vertices_num * 7 * sizeof(float), (void**)&data);
    memcpy(data, m_gl_preliminary_data, m_vertices_num * 7 * sizeof(float));
    for (int i = 0; i < vertex_num; ++i)
    {
        data[i * 7] *= v_scale_factors[i];
        data[i * 7 + 1] *= v_scale_factors[i];
    }
    ModelRemap(data, m_vertices_num);
    global_memory_pool.ReleaseMemory(data);
}

void PolygonTouchResponder::StaticDeform(float* v_scale_factors, unsigned vertex_num)
{
    if (vertex_num != m_vertices_num || v_scale_factors == nullptr) {
        return;
    }
    float* data;
    global_memory_pool.GetMemory(m_vertices_num * 7 * sizeof(float), (void**)&data);
    memcpy(data, m_gl_data, m_vertices_num * 7 * sizeof(float));
    for (int i = 0; i < vertex_num; ++i)
    {
        data[i * 7] *= v_scale_factors[i];
        data[i * 7 + 1] *= v_scale_factors[i];
    }
    ModelRemap(data, m_vertices_num);
    global_memory_pool.ReleaseMemory(data);
}

PolygonTouchResponder::~PolygonTouchResponder()
{
    global_memory_pool.ReleaseMemory(m_vertices_2d);
    global_memory_pool.ReleaseMemory(m_real_touch_area_vertices);
    global_memory_pool.ReleaseMemory(m_center_vectors);
    global_memory_pool.ReleaseMemory(m_gl_data);

    SAFE_DELETE(m_pShader);
    
    DetachEvent();
    DetachRendering();
    
    global_memory_pool.ReleaseMemory(m_gl_preliminary_data);
}














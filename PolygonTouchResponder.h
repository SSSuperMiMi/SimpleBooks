//
//  PolygonArea.h
//  Simple Books
//
//  Created by Allen Zhao on 5/15/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__PolygonArea__
#define __Simple_Books__PolygonArea__

#import "ViewController.h"
#import "enum.h"
#import "CTimer.h"
#import "Shader.h"
#import "Math.h"
#import "PhysicalInterpolation.h"
#import "Structs.h"

#define touch_area_magnification_factor_default         1.30f

struct TouchSpeedSignPackage;
struct uniform_speed_deform_args;
struct GravitationalField_deform_args;
class ConstantFieldModel;
class PolygonTouchResponder
{
public:
    bool m_pinching;
    bool m_rotating;
    bool m_paning;
    bool m_double_paning;
    bool m_enable_double_click;
    bool m_enable_two_finger_double_click;
    bool m_isVisible;
    bool m_isRendering;
    bool m_useShader;
    bool m_EnableDirectOSTouchMsg;
    
    unsigned m_vertices_num;
    unsigned m_current_event;
    unsigned m_double_click_time_interval_microseconds;
    unsigned m_positionSlot;
    unsigned m_colorSlot;
    unsigned m_RenderMode;
    
    float   m_touch_area_magnification_factor;
    float   m_scale_x;
    float   m_scale_y;
    float   m_angle;
    float   m_AngleAcceleration;
    float   m_AngleVelocity;
    float   s_color[4];
    float*  m_gl_data;
    float*  m_gl_preliminary_data;
    
    double m_time_last_frame;
    double deltaMS;
    
    static unsigned m_single_click_delay_ms_default;
    
    GLKMatrix4 m_modelViewMatrix;
    GLKMatrix4 m_projectionMatrix;
    GLKMatrix4 m_MVP;
    
    CGPoint*  m_real_touch_area_vertices;
    PolygonTouchResponder* m_pParent;
    vector_2d* m_center_vectors;
    point_2d*  m_vertices_2d;
    point_2d   m_center_point;
    point_2d   m_Position;
    point_2d   m_CurrentSingleClickPoint;
    
    vector_2d m_LineAcceleration;
    vector_2d m_LineVelocity;
    vector_2d m_GyrovBonus;
    vector_2d GyroscopeSpeedBonus;
    vector_2d m_Scale_Velocity;
    vector_2d M_Scale_Acceleration;
    vector_2d m_finger_touch_velocity;
    
    void Move(vector_2d);
    
    Stack     os_events;
    
    GLuint    m_vao_id;
    
    CTimer* m_pSingleClickTimer;
    CTimer* m_pTwoFingerSingleClickTimer;
    
    Shader* m_pShader;
    GLKBaseEffect* m_pBaseEffect;
    
    UIPinchGestureRecognizer* m_pinch_recognizer;
    UIRotationGestureRecognizer* m_rotate_recognizer;
    UIPanGestureRecognizer* m_pan_recognizer;
    UIPanGestureRecognizer* m_double_pan_recognizer;
    UIBezierPath* m_ClosePath;
    
    //
    // Message functions.
    //
    pfn_callback m_pfn_single_finger_single_click;
    pfn_callback m_pfn_single_finger_double_click;
    pfn_callback m_pfn_two_finger_single_click;
    pfn_callback m_pfn_two_finger_double_click;
    pfn_callback m_pfn_long_press_began;
    pfn_callback m_pfn_long_press_end;
    pfn_callback m_pfn_pinch;
    pfn_callback m_pfn_rotate;
    pfn_callback m_pfn_pan;
    pfn_callback m_pfn_double_pan;
    pfn_callback m_pfn_ConstantFieldModel_end;
    
    void EnableDirectOSTouchMsg(bool);
    void osTouchBegin(UITouch*);
    void osTouchMoved(UITouch*);
    void osTouchEnded(UITouch*);
    void SetLineVelocity(vector_2d);
    void SetLineAcceleration(vector_2d);
    void SetAngleVelocity(float);
    void SetAngleAcceleration(float);
    void SetRenderMode(unsigned);
    void SetRealTouchArea();
    void SetCenterVectors();
    void Rotate(float);
    void EnableShader(bool);
    void Transform();
    void EnableDoubleClick(bool);
    void EnableTwoFingerDoubleClick(bool);
    void SetDoubleClickTimeInterval(float);
    void ResetRealTouchArea(float);
    void OnTouchEvent(unsigned);
    void AttachEvent();
    void DetachEvent();
    void AttachRendering();
    void DetachRendering();
    void PrintGLDataLog();
    void PrintGLDataLog(float*);
    void ModelCopy(PolygonTouchResponder**);
    void ModelRemap(float*, unsigned);
    bool LoadShader(const char*, const char*, unsigned*);
    bool IsPointWithinThisArea(point_2d);
    bool RegisterCallback(pfn_callback, unsigned);
    bool SetColor(float*);
    bool FindOSEvent(UITouch*);
    TouchSpeedSignPackage* FindTSSP(UITouch*);
    void GetColor(float**);
    void SetScale(float, float);
    
    void AttachGyroPC();
    void DetachGyroPC();
    
    //
    // virtual functions.
    //
    virtual void SetVisible(bool);
    virtual void OSMove(point_2d, point_2d);
    virtual void Dock();
    virtual void UnDock();
    virtual void PrepareData();
    virtual void Rendering();
    virtual void PreparePosition();
    virtual void UpdateShaderValues();
    virtual void Update();
    virtual void SetPosition(point_2d);
    virtual void DoWithFingerTouchVelocity(point_2d StartPoint);
    
    ConstantFieldModel* ParabolaTracking;
    point_2d    GetPosition();
    vector_2d   GetScale();
    float       GetRotateAngle();
    GLKMatrix4  GetModelViewMatrix();

    uniform_speed_deform_args* pUniform_speed_deform_args;
    
    void StaticDeform(float*, unsigned);
    void OriginalStaticDeform(float*, unsigned);
    bool PolygonDeforming(uniform_speed_deform_args*);
    bool PolygonDeforming(ConstantFieldModel*);
    
    void PrepareModel();
    void VertexCompensation(unsigned);
    void InsertCompensationVertex(float*, unsigned);
    void ToBeStatic();
    
    static void DelaySingleClickAfterSeconds(void*);
    static void DelayTwoFingerSingleClickAfterSeconds(void*);
    static bool CreatePolygonTouchResponder(float*, unsigned, PolygonTouchResponder**);
    
    PolygonTouchResponder(float*, unsigned);
    ~PolygonTouchResponder();
};

#endif /* defined(__Simple_Books__PolygonArea__) */

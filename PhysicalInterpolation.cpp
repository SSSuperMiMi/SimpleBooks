//
//  PhysicalInterpolation.cpp
//  Simple Books
//
//  Created by Allen Zhao on 7/9/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "PhysicalInterpolation.h"
#import "MemoryPool.h"

float DeformBase::GetAngleVelocity()
{
    float rate = (float)m_DeformingKeyframe / m_Frames;
    return (m_begin_AngleVelocity + rate * (m_TargetAngleVelocity - m_begin_AngleVelocity));
}

void DeformBase::SetBeginGL_data(float* gl_data)
{
    global_memory_pool.GetMemory(m_pTarget->m_vertices_num * 7 * sizeof(float), (void**)&m_begin_gl_data);
    memcpy(m_begin_gl_data, gl_data, m_pTarget->m_vertices_num * 7 * sizeof(float));
}

float DeformBase::GetAngleAcceleration()
{
    float rate = (float)m_DeformingKeyframe / m_Frames;
    return (m_begin_AngleAcceleration + rate * (m_TargetAngleAcceleration - m_begin_AngleAcceleration));
}

DeformBase::DeformBase():m_pTarget(nullptr), m_self_transform(false), m_self(nullptr), m_begin_gl_data(nullptr)
{
    memset(&m_self_end_size, 0, sizeof(vector_2d));
    memset(&m_self_begin_size, 0, sizeof(vector_2d));
    
    m_Frames = m_Time = m_DeformingKeyframe = m_TargetAngleVelocity = m_TargetAngleAcceleration = m_MS = 0;
    m_begin_AngleAcceleration = m_begin_AngleVelocity = 0;
    m_AngleVelocity_Interpolation = m_AngleAcceleration_Interpolation = false;
}

DeformBase::~DeformBase()
{
}

vector_2d DeformBase::GetSize()
{
    vector_2d v;
    float rate = (float)m_DeformingKeyframe / m_Frames;
    v.dx = m_self_begin_size.dx + rate * (m_self_end_size.dx - m_self_begin_size.dx);
    v.dy = m_self_begin_size.dy + rate * (m_self_end_size.dy - m_self_begin_size.dy);
    
    return v;
}

uniform_speed_deform_args::~uniform_speed_deform_args()
{
}

uniform_speed_deform_args::uniform_speed_deform_args(point_2d pos_begin, point_2d pos_end, unsigned fps, float begin_AngleAcceleration, float begin_AngleVelocity,float TargetAngleVelocity, float TargetAngleAcceleration, vector_2d self_begin_size, vector_2d self_end_size, PolygonTouchResponder* Target, PolygonTouchResponder* Self)
{
    m_Frames = fps;
    m_TargetAngleVelocity = TargetAngleVelocity;
    m_TargetAngleAcceleration = TargetAngleAcceleration;
    m_pTarget = Target;
    m_DeformingKeyframe = 0;
    m_MS = 0;
    m_Time = m_Frames * 1000.0f / 60;
    m_begin_AngleVelocity = begin_AngleVelocity;
    m_begin_AngleAcceleration = begin_AngleAcceleration;
    memcpy(&m_p1, &pos_end, sizeof(point_2d));
    memcpy(&m_p0, &pos_begin, sizeof(point_2d));
    if (TargetAngleVelocity != 0.0f) {
        m_AngleVelocity_Interpolation = true;
    }
    if (TargetAngleAcceleration != 0.0f) {
        m_AngleAcceleration_Interpolation = true;
    }
    m_self = Self;
}

ConstantFieldModel::~ConstantFieldModel()
{
}

ConstantFieldModel::ConstantFieldModel(point_2d pos_begin, point_2d pos_end, float v0y_begin, unsigned fps, float begin_AngleAcceleration, float begin_AngleVelocity,float TargetAngleVelocity, float TargetAngleAcceleration, vector_2d self_begin_size, vector_2d self_end_size, PolygonTouchResponder* Target, PolygonTouchResponder* Self)
{
    m_pTarget = Target;
    memcpy(&m_p1, &pos_end, sizeof(point_2d));
    float m_total_time = fps * 1000.0f / 60.0f;
    memcpy(&m_p0, &pos_begin, sizeof(point_2d));
    m_v0.dx = (m_p1.x - m_p0.x) / m_total_time;
    m_v0.dy = v0y_begin;
    
    m_Frames = fps;
    m_a.dx = 0.0f;
    m_a.dy = 2 * (m_p1.y - m_p0.y - m_v0.dy * m_total_time) / powf(m_total_time, 2);
    
    m_TargetAngleVelocity = TargetAngleVelocity;
    m_TargetAngleAcceleration = TargetAngleAcceleration;
    
    memcpy(&m_self_end_size, &self_end_size, sizeof(vector_2d));
    memcpy(&m_self_begin_size, &self_begin_size, sizeof(vector_2d));
    m_DeformingKeyframe = 0;
    m_MS = 0;
    m_Time = m_Frames * 1000.0f / 60;
    m_begin_AngleVelocity = begin_AngleVelocity;
    m_begin_AngleAcceleration = begin_AngleAcceleration;
    if (TargetAngleVelocity != 0.0f) {
        m_AngleVelocity_Interpolation = true;
    }
    if (TargetAngleAcceleration != 0.0f) {
        m_AngleAcceleration_Interpolation = true;
    }
    m_self = Self;
}

vector_2d ConstantFieldModel::GetV()
{
    float t = (m_DeformingKeyframe * 1000) / 60.0f;
    vector_2d v;
    v.dx = m_v0.dx;
    v.dy = m_v0.dy + m_a.dy * t;
    
    return v;
}

point_2d ConstantFieldModel::GetPos()
{
    float t = (m_DeformingKeyframe * 1000) / 60.0f;
    point_2d p;
    p.x = m_v0.dx * t + m_p0.x;
    p.y = m_a.dy * powf(t, 2.0f) / 2.0f + m_v0.dy * t + m_p0.y;
    
    return p;
}

point_2d uniform_speed_deform_args::GetPos()
{
    float rate = (float)m_DeformingKeyframe / m_Frames;
    point_2d p;
    p.x = m_p0.x + (m_p1.x - m_p0.x) * rate;
    p.y = m_p0.y + (m_p1.y - m_p0.y) * rate;
    return p;
}

void uniform_speed_deform_args::Reset()
{
    m_Frames = m_Time = m_DeformingKeyframe = m_TargetAngleVelocity = m_TargetAngleAcceleration = m_MS = m_self_end_size.dx = m_self_end_size.dy = m_self_begin_size.dx = m_self_begin_size.dy = 0;
    
    m_pTarget = nullptr;
    m_self = nullptr;
    m_begin_AngleAcceleration = m_begin_AngleVelocity = 0;
    m_AngleVelocity_Interpolation = m_AngleAcceleration_Interpolation = false;
    global_memory_pool.ReleaseMemory(m_begin_gl_data);
    m_begin_gl_data = nullptr;
}

void ConstantFieldModel::Reset()
{
    m_Frames = m_Time = m_DeformingKeyframe = m_TargetAngleVelocity = m_TargetAngleAcceleration = m_MS = m_self_end_size.dx = m_self_end_size.dy = m_self_begin_size.dx = m_self_begin_size.dy = m_v0.dx = m_v0.dy = m_p0.x = m_p0.y = m_p1.x = m_p1.y = m_a.dx = m_a.dy = 0;
    
    m_pTarget = nullptr;
    m_self = nullptr;
    m_begin_AngleAcceleration = m_begin_AngleVelocity = 0;
    m_AngleVelocity_Interpolation = m_AngleAcceleration_Interpolation = false;
    global_memory_pool.ReleaseMemory(m_begin_gl_data);
    m_begin_gl_data = nullptr;
}



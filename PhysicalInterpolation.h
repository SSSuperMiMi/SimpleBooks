//
//  PhysicalInterpolation.h
//  Simple Books
//
//  Created by Allen Zhao on 7/9/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__PhysicalInterpolation__
#define __Simple_Books__PhysicalInterpolation__

#include <stdio.h>
#import "Math.h"
#import "PolygonTouchResponder.h"

class PolygonTouchResponder;

struct DeformBase
{
    DeformBase();
    ~DeformBase();
    
    unsigned m_Frames;
    double   m_Time;
    unsigned m_DeformingKeyframe;
    
    float m_TargetAngleVelocity;
    float m_TargetAngleAcceleration;
    float m_begin_AngleVelocity;
    float m_begin_AngleAcceleration;
    float GetAngleVelocity();
    float GetAngleAcceleration();
    float  m_MS;
    vector_2d m_self_end_size;
    vector_2d m_self_begin_size;
    vector_2d GetSize();
    bool m_self_transform;
    bool m_AngleVelocity_Interpolation;
    bool m_AngleAcceleration_Interpolation;
    float* m_begin_gl_data;
    void SetBeginGL_data(float*);
    
    point_2d m_p0;
    point_2d m_p1;
    
    PolygonTouchResponder* m_self;
    PolygonTouchResponder* m_pTarget;
};

struct uniform_speed_deform_args : public DeformBase
{
    uniform_speed_deform_args(point_2d pos_begin, point_2d pos_end, unsigned fps, float begin_AngleAcceleration, float begin_AngleVelocity,float TargetAngleVelocity, float TargetAngleAcceleration, vector_2d self_begin_size, vector_2d self_end_size, PolygonTouchResponder* Target, PolygonTouchResponder* Self);
    void Reset();
    point_2d GetPos();
    
    ~uniform_speed_deform_args();
};

struct ConstantFieldModel : public DeformBase
{
    ~ConstantFieldModel();
    ConstantFieldModel(point_2d pos_begin, point_2d pos_end, float v0y_begin, unsigned fps, float begin_AngleAcceleration, float begin_AngleVelocity,float TargetAngleVelocity, float TargetAngleAcceleration, vector_2d self_begin_size, vector_2d self_end_size, PolygonTouchResponder* Target, PolygonTouchResponder* Self);
    
    point_2d GetPos();
    vector_2d GetV();
    vector_2d m_v0;
    vector_2d m_a;
    
    void Reset();
};
#endif /* defined(__Simple_Books__PhysicalInterpolation__) */

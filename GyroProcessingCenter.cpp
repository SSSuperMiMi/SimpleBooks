//
//  GyroProcessingCenter.cpp
//  Simple Books
//
//  Created by Allen Zhao on 7/24/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "GyroProcessingCenter.h"
#import "Gyro.h"

GyroPC _GyroPC_;

float GyroPC::max_bonus_v_x = 0.0003;
float GyroPC::max_bonus_v_y = 0.0003;

double GyroPC::ax = 0;
double GyroPC::ay = 0;
double GyroPC::az = 0;

GyroPC::GyroPC()
{
}

void GyroPC::Add(PolygonTouchResponder* pTarget)
{
    objs.PushAtHead(pTarget);
}

void GyroPC::Remove(PolygonTouchResponder* pTarget)
{
    objs.DeleteObject(pTarget);
}

void GyroPC::Run()
{
    unsigned ctl_num = objs.GetLength();
    if (ctl_num == 0) {
        return;
    }
    
    for(int i = 0; i < ctl_num; ++i)
    {
        PolygonTouchResponder* pPolygonTouchResponder = ((PolygonTouchResponder*)objs[i]);
        pPolygonTouchResponder->m_LineVelocity = Vminus(pPolygonTouchResponder->m_LineVelocity, pPolygonTouchResponder->m_GyrovBonus);
        pPolygonTouchResponder->m_GyrovBonus.dx =  max_bonus_v_x * sinf(GyroData::roll);
        pPolygonTouchResponder->m_GyrovBonus.dy = -max_bonus_v_y * sinf(GyroData::pinch);
        pPolygonTouchResponder->m_LineVelocity = VAdd(pPolygonTouchResponder->m_LineVelocity, pPolygonTouchResponder->m_GyrovBonus);
    }
}
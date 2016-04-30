//
//  GraphicsProcessingCenter.cpp
//  Simple Books
//
//  Created by Allen Zhao on 7/23/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "GraphicsProcessingCenter.h"
#import "ios_sys.h"
#import "SimpleBooks.h"
#import "GyroProcessingCenter.h"

GPC _gpc_;

void GPC::AddStaticObstacles(Circle* pTarget)
{
    StaticObstacles.PushAtHead(pTarget);
}

void GPC::RemoveStaticObstacles(Circle* pTarget)
{
    StaticObstacles.DeleteObject(pTarget);
}

void GPC::Add(Circle* pTarget)
{
    objs.PushAtHead(pTarget);
}

void GPC::Remove(Circle* pTarget)
{
    if(pTarget == nullptr)return;
    
    objs.DeleteObject(pTarget);
}

void GPC::Run()
{
    unsigned objs_num = objs.GetLength();
    if (objs_num == 0) {
        return;
    }
    for(int i = 0; i < objs_num - 1; ++i)
    {
        Circle* c1 = ((Circle*)objs[i]);
        for (int j = i + 1; j < objs_num; ++j) {
            Circle* c2 = ((Circle*)objs[j]);
            
            float Distance = sqrtf(
                                   powf(c2->m_Position.x - c1->m_Position.x, 2) + powf(c2->m_Position.y - c1->m_Position.y, 2));
            vector_2d c1toc2 = MakeVector2d(c2->m_Position.x - c1->m_Position.x, c2->m_Position.y - c1->m_Position.y);
            vector_2d ProjectionV1 = GetProjectionVector(c1->m_LineVelocity, c1toc2);
            vector_2d ProjectionV2 = GetProjectionVector(c2->m_LineVelocity, c1toc2);
            vector_2d deltaV = Vminus(ProjectionV2, ProjectionV1);
            
            float angle = GetvAngle(c1toc2, deltaV);
            
            if (Distance <= (c1->radius + c2->radius) && angle != 0.0f) {
                c1->m_LineVelocity.dx += c2->m_LineVelocity.dx;
                c2->m_LineVelocity.dx  = c1->m_LineVelocity.dx - c2->m_LineVelocity.dx;
                c1->m_LineVelocity.dx -= c2->m_LineVelocity.dx;
                
                c1->m_LineVelocity.dy += c2->m_LineVelocity.dy;
                c2->m_LineVelocity.dy  = c1->m_LineVelocity.dy - c2->m_LineVelocity.dy;
                c1->m_LineVelocity.dy -= c2->m_LineVelocity.dy;
            }
        }
    }
    
    unsigned static_obstacle_objs_num = StaticObstacles.GetLength();
    for (int i = 0; i < static_obstacle_objs_num; ++i) {
        Circle* Obsatcle = ((Circle*)StaticObstacles[i]);
        for (int j = 0; j < objs_num; ++j) {
            Circle* c = ((Circle*)objs[j]);
            {
                float Distance = sqrtf(
                                       powf(Obsatcle->m_Position.x - c->m_Position.x, 2) + powf(Obsatcle->m_Position.y - c->m_Position.y, 2));
                vector_2d c1toc2 = MakeVector2d(c->m_Position.x - Obsatcle->m_Position.x, c->m_Position.y - Obsatcle->m_Position.y);
                float angle = GetvAngle(c1toc2, GetProjectionVector(c->m_LineVelocity, c1toc2));
                if (Distance <= (Obsatcle->radius + c->radius) && angle != 0.0f) {
                    vector_2d u = UnitVector(c1toc2);
                    float base_v = (float)GetRandNum(3, 5) / 10000;
                    c->m_LineVelocity.dx = u.dx * base_v;
                    c->m_LineVelocity.dy = u.dy * base_v;
                    c->m_GyrovBonus.dx = c->m_GyrovBonus.dy = 0;
                    if (fabsf(c->m_LineVelocity.dx) < GyroPC::max_bonus_v_x) {
                        if (c->m_LineVelocity.dx > 0) {
                            c->m_LineVelocity.dx += GyroPC::max_bonus_v_x;
                        }
                        else
                        {
                            c->m_LineVelocity.dx -= GyroPC::max_bonus_v_x;
                        }
                    }
                    if (fabsf(c->m_LineVelocity.dy) < GyroPC::max_bonus_v_y) {
                        if (c->m_LineVelocity.dy > 0) {
                            c->m_LineVelocity.dy += GyroPC::max_bonus_v_y;
                        }
                        else
                        {
                            c->m_LineVelocity.dy -= GyroPC::max_bonus_v_y;
                        }
                    }
                }
            }
        }
    }
    
    for (int j = 0; j < objs_num; ++j) {
        Circle* c = ((Circle*)objs[j]);
        {
            if (c->m_Position.x - c->radius < -ios_sys::gl_bound.x)
            {
                if (c->m_LineVelocity.dx < 0) {
                    vector_2d BoundaryNormalVector = MakeVector2d(1, 0);
                    c->m_LineVelocity = GetReflectionVector(
                                                            c->m_LineVelocity, BoundaryNormalVector);
                    
                    c->m_LineVelocity.dx += c->m_GyrovBonus.dx * 2;
                }
            }
            else if(c->m_Position.x + c->radius > ios_sys::gl_bound.x)
            {
                if (c->m_LineVelocity.dx > 0) {
                    vector_2d BoundaryNormalVector = MakeVector2d(-1, 0);
                    c->m_LineVelocity = GetReflectionVector(
                                                            c->m_LineVelocity, BoundaryNormalVector);
                    c->m_LineVelocity.dx += c->m_GyrovBonus.dx * 2;
                }
            }
            else if(c->m_Position.y + c->radius > ios_sys::gl_bound.y)
            {
                if (c->m_LineVelocity.dy > 0) {
                    vector_2d BoundaryNormalVector = MakeVector2d(0, -1);
                    c->m_LineVelocity = GetReflectionVector(
                                                            c->m_LineVelocity, BoundaryNormalVector);
                    c->m_LineVelocity.dy += c->m_GyrovBonus.dy * 2;
                }
            }
            else if(c->m_Position.y - c->radius < -ios_sys::gl_bound.y)
            {
                if (c->m_LineVelocity.dy < 0) {
                    vector_2d BoundaryNormalVector = MakeVector2d(0, 1);
                    c->m_LineVelocity = GetReflectionVector(
                                                            c->m_LineVelocity, BoundaryNormalVector);
                    c->m_LineVelocity.dy += c->m_GyrovBonus.dy * 2;
                }
            }
        }
    }
}







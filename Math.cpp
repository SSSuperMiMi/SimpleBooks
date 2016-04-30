//
//  Math.cpp
//  Simple Books
//
//  Created by Allen Zhao on 5/23/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "Math.h"
#import "MemoryPool.h"
#import "Macro.h"

// 旋转以逆时针为正.
unsigned math::uDifferenceAbs(unsigned n1, unsigned n2)
{
    return (n1 > n2) ? (n1 - n2) : (n2 - n1);
}

void math::GLKMatrix4LoadIdentity(GLKMatrix4* result)
{
    memset(result, 0x0, sizeof(GLKMatrix4));
    
    result->m00 = 1.0f;
    result->m11 = 1.0f;
    result->m22 = 1.0f;
    result->m33 = 1.0f;
}

unsigned math::ksNextPot(unsigned n)
{
    --n;
    unsigned offset = 0x10;
    while (offset) {
        n |= n >> offset;
        offset >>= 1;
    }
    ++n;
    return n;
}

point_2d MakePoint2D(float x, float y)
{
    point_2d p;
    p.x = x;
    p.y = y;
    
    return p;
}

point_3d MakePoint2D(float x, float y, float z)
{
    point_3d p;
    
    p.x = x;
    p.y = y;
    p.z = z;
    
    return p;
}

vector_2d MakeVector2d(float dx, float dy)
{
    vector_2d v;
    v.dx = dx;
    v.dy = dy;
    
    return v;
}

vector_3d MakeVector3d(float dx, float dy, float dz)
{
    vector_3d v;
    
    v.dx = dx;
    v.dy = dy;
    v.dz = dz;
    
    return v;
}

float counter_clockwise_rotation_angle(vector_2d v1, vector_2d v2)
{
    if (v1.dx == 0) {
        if (v2.dx == 0) {
            return 0;
        }
        else
        {
            float θ1 = M_PI_2;
            float θ2;
            
            if (v2.dy >= 0) {
                θ2 = GetvAngle(v2, MakeVector2d(1, 0));
            }
            else
            {
                θ2 = M_PI * 2 -  GetvAngle(v2, MakeVector2d(1, 0));
            }
            
            return GetBaseAngle(θ2 - θ1);
        }
    }
    else
    {
        if (v2.dx == 0) {
            float θ1;
            if (v1.dy >= 0) {
                θ1 = GetvAngle(v1, MakeVector2d(1, 0));
            }
            else
            {
                θ1 = M_PI * 2 -  GetvAngle(v1, MakeVector2d(1, 0));
            }
            
            float θ2 = M_PI_2;
            return GetBaseAngle(θ2 - θ1);
        }
        else
        {
            float θ1, θ2;
            if (v1.dy >= 0) {
                θ1 = GetvAngle(v1, MakeVector2d(1, 0));
            }
            else
            {
                θ1 = M_PI * 2 -  GetvAngle(v1, MakeVector2d(1, 0));
            }
            
            if (v2.dy >= 0) {
                θ2 = GetvAngle(v2, MakeVector2d(1, 0));
            }
            else
            {
                θ2 = M_PI * 2 -  GetvAngle(v2, MakeVector2d(1, 0));
            }

            return GetBaseAngle(θ2 - θ1);
        }
    }
}

float GetvAngle(vector_2d v1, vector_2d v2)
{
    float l = sqrtf(powf(v1.dx, 2) + powf(v1.dy, 2)) * sqrtf(powf(v2.dx, 2) + powf(v2.dy, 2));
    float dp = v1.dx * v2.dx + v1.dy * v2.dy;
    
    return acosf(dp / l);
}

float GetBaseAngle(float angle)
{
    while (true) {
        if (angle >=0 && angle <= 2 * M_PI) {
            return angle;
        }
        if (angle <= 0) {
            angle += 2 * M_PI;
        }
        else
        {
            angle -= 2 * M_PI;
        }
    }
}

vector_2d GetReflectionVector(vector_2d v, vector_2d nv)
{
    vector_2d unit_nv = UnitVector(nv);
    
    float vmn = v2dDot(v, unit_nv);
    float v2mn = vmn * 2;
    vector_2d v2 = VmultiplyNum(unit_nv, v2mn);
    
    return Vminus(v, v2);

}

vector_2d VmultiplyNum(vector_2d v, float n)
{
    return MakeVector2d(v.dx * n, v.dy * n);
}

vector_2d Vminus(vector_2d v1, vector_2d v2)
{
    return MakeVector2d(v1.dx - v2.dx, v1.dy - v2.dy);
}

vector_2d VAdd(vector_2d v1, vector_2d v2)
{
    return MakeVector2d(v1.dx + v2.dx, v1.dy + v2.dy);
}

vector_2d UnitVector(vector_2d v)
{
    float l = lvector2d(v);
    return MakeVector2d(v.dx / l, v.dy / l);
}

float lvector2d(vector_2d v)
{
    return sqrtf((powf(v.dx, 2) + powf(v.dy, 2)));
}

float v2dDot(vector_2d v1, vector_2d v2)
{
    return v1.dx * v2.dx + v1.dy * v2.dy;
}

vector_2d GetBaseV(float angle)
{
    return MakeVector2d(cosf(angle), sinf(angle));
}

int GetRandNum(int n1, int n2)
{
    return n1 + rand() % (n2 - n1 + 1);
}

vector_2d GetProjectionVector(vector_2d v1, vector_2d v2)
{
    float ProjectionLength = (v1.dx * v2.dx + v1.dy * v2.dy) / sqrtf(powf(v2.dx, 2) + powf(v2.dy, 2));
    return VmultiplyNum(UnitVector(v2), ProjectionLength);
}

vector_2d GetVerticalVector(vector_2d v)
{
    return MakeVector2d(-v.dy, v.dx);
}

vector_2d GetVReverse(vector_2d v)
{
    return MakeVector2d(-v.dx, -v.dy);
}

point_2d PointRotation(point_2d Revolution_center, point_2d target, float angle)
{
    point_2d ret;
    
    float posx = target.x;
    float posy = target.y;
    
    ret.x = (posx - Revolution_center.x)  * cosf(angle) -
    (posy - Revolution_center.y) * sinf(angle) + Revolution_center.x;
    ret.y = (posx - Revolution_center.x) * sinf(angle) +
    (posy - Revolution_center.y) * cosf(angle) + Revolution_center.y;
    
    return ret;
}

void GetQuarterStartMonthAndEndMonth(unsigned Month, unsigned& StartM, unsigned& EndM)
{
    if (Month < 1 || Month > 12) {
        StartM = EndM = 0;return;
    }
    
    unsigned M = Month;
    while (true) {
        if (M % 3 == 1) {
            StartM = M;break;
        }
        --M;
    }
    M = Month;
    while (true) {
        if (M % 3 == 0) {
            EndM = M;break;
        }
        ++M;
    }
}

float GetDistanceSquared(point_2d p1, point_2d p2)
{
    return powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2);
}

float GetDistance(point_2d p1, point_2d p2)
{
    return sqrtf(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2));
}

bool IsEqual(point_2d p1, point_2d p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

vector_2d GetRepulsion(point_2d source, point_2d Particle, float G)
{
    return GetVReverse(GetGravity(source, Particle, G));
}

vector_2d GetGravity(point_2d source/* 场源 */, point_2d Particle/* 质点 */, float G/* 引力常数 */)
{
    if(IsEqual(source, Particle) == true && G != 0)
    {
        return MakeVector2d(invalid_value, invalid_value);
    }
    if(G == 0)
    {
        return MakeVector2d(0, 0);
    }
    
    float a = G / GetDistanceSquared(source, Particle);
    
    vector_2d GV = MakeVector2d(source.x - Particle.x, source.y - Particle.y);
    vector_2d UGV = UnitVector(GV);
    
    return MakeVector2d(UGV.dx * a, UGV.dy * a);
}

void GetCircle(float radius, float thickness, float* color, float** data)
{
    if (radius <= 0 || thickness <= 0 || color == nullptr) {
        *data = 0;
        return;
    }
    float r = color[0];
    float g = color[1];
    float b = color[2];
    float a = color[3];
    
    float*& gl_data = *data;
    float ProgressiveAngle = 0;
    for(int i = 0; i < default_precision_of_circle_double / 2; ++i)
    {
        gl_data[i * 14 + 0] = (radius + thickness) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 1] = (radius + thickness) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 2] = 0;
        gl_data[i * 14 + 3] = r;
        gl_data[i * 14 + 4] = g;
        gl_data[i * 14 + 5] = b;
        gl_data[i * 14 + 6] = a;
        
        gl_data[i * 14 + 7] = (radius) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 8] = (radius) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 9] = 0;
        gl_data[i * 14 + 10] = r;
        gl_data[i * 14 + 11] = g;
        gl_data[i * 14 + 12] = b;
        gl_data[i * 14 + 13] = a;
        
        ProgressiveAngle += M_PI * 4 / (default_precision_of_circle_double - 2);
    }
}

void GetSolidRound(float radius, float* color, float** data)
{
    // default_precision_of_circle + 1个顶点.
    float r = color[0];
    float g = color[1];
    float b = color[2];
    float a = color[3];
    float ProgressiveAngle = 0;
    float*& gl_data = *data;
    for (int i = 0; i < default_precision_of_circle + 1; ++i) {
        if (i == 0) {
            gl_data[i * 7 + 0] = 0;
            gl_data[i * 7 + 1] = 0;
            gl_data[i * 7 + 2] = 0;
            gl_data[i * 7 + 3] = r;
            gl_data[i * 7 + 4] = g;
            gl_data[i * 7 + 5] = b;
            gl_data[i * 7 + 6] = a;
            continue;
        }
        gl_data[i * 7 + 0] = radius * cosf(ProgressiveAngle);
        gl_data[i * 7 + 1] = radius * sinf(ProgressiveAngle);
        gl_data[i * 7 + 2] = 0;
        gl_data[i * 7 + 3] = r;
        gl_data[i * 7 + 4] = g;
        gl_data[i * 7 + 5] = b;
        gl_data[i * 7 + 6] = a;
        
        ProgressiveAngle += M_PI * 2 / (default_precision_of_circle - 2);
    }
}













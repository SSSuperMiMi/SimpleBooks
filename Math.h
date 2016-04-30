//
//  Math.h
//  Simple Books
//
//  Created by Allen Zhao on 5/23/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__Math__
#define __Simple_Books__Math__

#include <stdio.h>
#import <GLKit/GLKit.h>
#include "DataStructure.h"

namespace math {
    void GLKMatrix4LoadIdentity(GLKMatrix4*);
    unsigned ksNextPot(unsigned);
    unsigned uDifferenceAbs(unsigned, unsigned);
}

struct point_2d
{
    float x;
    float y;
};

struct point_3d
{
    float x;
    float y;
    float z;
};

struct vector_2d
{
    float dx;
    float dy;
};

struct vector_3d
{
    float dx;
    float dy;
    float dz;
};

point_2d MakePoint2D(float, float);
point_3d MakePoint3D(float, float, float);
vector_2d MakeVector2d(float, float);
vector_3d MakeVector3d(float, float, float);
float GetvAngle(vector_2d, vector_2d);
float GetBaseAngle(float);
float counter_clockwise_rotation_angle(vector_2d, vector_2d);
vector_2d GetReflectionVector(vector_2d, vector_2d);
vector_2d GetBaseV(float);
float v2dDot(vector_2d, vector_2d);
float lvector2d(vector_2d);
vector_2d UnitVector(vector_2d);
vector_2d VmultiplyNum(vector_2d, float);
vector_2d VAdd(vector_2d, vector_2d);
vector_2d Vminus(vector_2d, vector_2d);
vector_2d GetProjectionVector(vector_2d, vector_2d);
int GetRandNum(int, int);
vector_2d GetVerticalVector(vector_2d);
vector_2d GetVReverse(vector_2d);
point_2d  PointRotation(point_2d center, point_2d target, float angle);
void GetQuarterStartMonthAndEndMonth(unsigned, unsigned&, unsigned&);
void GetSolidRound(float, float*, float**);
void GetCircle(float, float, float*, float**);
bool IsEqual(point_2d, point_2d);
float GetDistance(point_2d, point_2d);
float GetDistanceSquared(point_2d, point_2d);
vector_2d GetGravity(point_2d, point_2d, float);
vector_2d GetRepulsion(point_2d, point_2d, float);

#endif /* defined(__Simple_Books__Math__) */

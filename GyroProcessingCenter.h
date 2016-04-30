//
//  GyroProcessingCenter.h
//  Simple Books
//
//  Created by Allen Zhao on 7/24/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__GyroProcessingCenter__
#define __Simple_Books__GyroProcessingCenter__

#include <stdio.h>
#import "DataStructure.h"
#import "PolygonTouchResponder.h"

class GyroPC
{
public:
    GyroPC();
    
    static double ax;
    static double ay;
    static double az;
    
    static float max_bonus_v_x;
    static float max_bonus_v_y;
    
    void Run();
    
    Deque objs;
    
    void Add(PolygonTouchResponder*);
    void Remove(PolygonTouchResponder*);
};

extern GyroPC _GyroPC_;

#endif /* defined(__Simple_Books__GyroProcessingCenter__) */

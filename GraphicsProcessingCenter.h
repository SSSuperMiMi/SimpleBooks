//
//  GraphicsProcessingCenter.h
//  Simple Books
//
//  Created by Allen Zhao on 7/23/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__GraphicsProcessingCenter__
#define __Simple_Books__GraphicsProcessingCenter__

#include <stdio.h>
#import "DataStructure.h"
#import "SpecialPolygon.h"

class GPC
{
public:
    void Run();
    
    Deque objs;
    Deque StaticObstacles;
    
    void AddStaticObstacles(Circle*);
    void RemoveStaticObstacles(Circle*);
    
    void Add(Circle*);
    void Remove(Circle*);
};

extern GPC _gpc_;

#endif /* defined(__Simple_Books__GraphicsProcessingCenter__) */

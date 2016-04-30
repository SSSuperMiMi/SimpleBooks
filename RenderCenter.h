//
//  RenderCenter.h
//  Simple Books
//
//  Created by Allen Zhao on 6/14/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__RenderCenter__
#define __Simple_Books__RenderCenter__

#include <stdio.h>
#include "DataStructure.h"
#include "PolygonTouchResponder.h"

class RenderQueue{
public:
    RenderQueue();
    Queue objs;
    
    void Add(PolygonTouchResponder*);
    void Remove(PolygonTouchResponder*);
    void Rendering();
    void Update();
};

extern RenderQueue render;
#endif /* defined(__Simple_Books__RenderCenter__) */

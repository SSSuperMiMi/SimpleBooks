//
//  RenderCenter.cpp
//  Simple Books
//
//  Created by Allen Zhao on 6/14/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "RenderCenter.h"
#import "SimpleBooks.h"

RenderQueue render;
RenderQueue::RenderQueue()
{
}

void RenderQueue::Update()
{
    pSimpleBooks->Update();
}

void RenderQueue::Add(PolygonTouchResponder* pPolygon)
{
    if(pPolygon == NULL)return;
    pPolygon->m_pBaseEffect = VC.baseEffect;
    objs.PushAtEnd(pPolygon);
}

void RenderQueue::Remove(PolygonTouchResponder* pPolygon)
{
    if(pPolygon == NULL)return;
    pPolygon->m_pBaseEffect = nullptr;
    objs.DeleteObject(pPolygon);
}

void RenderQueue::Rendering()
{
    for(int i = 0; i < objs.GetLength(); ++i)
    {
        GLKMatrix4 saveModelViewMatrix = VC.baseEffect.transform.modelviewMatrix;
        PolygonTouchResponder* pPolygonTouchResponder = ((PolygonTouchResponder*)objs[i]);
        pPolygonTouchResponder->PrepareData();
        pPolygonTouchResponder->Rendering();
        VC.baseEffect.transform.modelviewMatrix = saveModelViewMatrix;
    }
}
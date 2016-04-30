//
//  TouchMsgCenter.cpp
//  Simple Books
//
//  Created by Allen Zhao on 5/15/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "TouchMsgCenter.h"
#include "ios_sys.h"

TouchMsgCenter MsgCenter;
TouchMsgCenter::~TouchMsgCenter()
{
}

TouchMsgCenter::TouchMsgCenter()
{
}

void TouchMsgCenter::direct_event(os_package osp)
{
    CGPoint point=[osp.touch locationInView:[osp.touch view]];
    point_2d gl_point = ios_sys::logic_coordinate_to_gl_coordinate(MakePoint2D(point.x, point.y));

    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if (pPolygon->m_EnableDirectOSTouchMsg == false) {
            continue;
        }
        if (osp.Type == _touchesBegan) {
            if(pPolygon->IsPointWithinThisArea(gl_point))
            {
                pPolygon->osTouchBegin(osp.touch);
            }
            continue;
        }
        if (osp.Type == _touchesMoved) {
            if (pPolygon->FindOSEvent(osp.touch)) {
                pPolygon->osTouchMoved(osp.touch);
            }
            continue;
        }
        if (osp.Type == _touchesEnded) {
            if (pPolygon->FindOSEvent(osp.touch)) {
                pPolygon->osTouchEnded(osp.touch);
            }
            continue;
        }
        if (osp.Type == _touchesCancelled) {
            if (pPolygon->FindOSEvent(osp.touch)) {
                pPolygon->osTouchEnded(osp.touch);
            }
        }
    }
}

void TouchMsgCenter::double_pan(UIPanGestureRecognizer* recognizer, point_2d point1, point_2d point2)
{
    bool event_began = !
    (point1.x == invalid_value &&
     point1.y == invalid_value &&
     point2.x == invalid_value &&
     point2.y == invalid_value);
    
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(event_began == true)
        {
            if(pPolygon->IsPointWithinThisArea(point1) &&
               pPolygon->IsPointWithinThisArea(point2))
            {
                pPolygon->m_double_paning = true;
                pPolygon->m_double_pan_recognizer = recognizer;
                return pPolygon->OnTouchEvent(enum_double_pan);
            }
        }
        else
        {
            if(pPolygon->m_double_pan_recognizer != NULL)
            {
                return ((PolygonTouchResponder*)Polygons[i])->OnTouchEvent(enum_double_pan);
            }
        }
    }
}

void TouchMsgCenter::pan(
                        UIPanGestureRecognizer* recognizer,
                        point_2d point)
{
    bool event_began = !
    (point.x == invalid_value &&
     point.y == invalid_value);
    
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(event_began == true)
        {
            if(pPolygon->IsPointWithinThisArea(point))
            {
                pPolygon->m_paning = true;
                pPolygon->m_pan_recognizer = recognizer;
                return pPolygon->OnTouchEvent(enum_pan);
            }
        }
        else
        {
            if(pPolygon->m_pan_recognizer != NULL)
            {
                return ((PolygonTouchResponder*)Polygons[i])->OnTouchEvent(enum_pan);
            }
        }
    }
}

void TouchMsgCenter::rotating(UIRotationGestureRecognizer* recognizer, point_2d point1, point_2d point2)
{
    bool event_began = !
    (point1.x == invalid_value &&
     point1.y == invalid_value &&
     point2.x == invalid_value &&
     point2.y == invalid_value);
    
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(event_began == true)
        {
            if(pPolygon->IsPointWithinThisArea(point1) &&
               pPolygon->IsPointWithinThisArea(point2))
            {
                pPolygon->m_rotating = true;
                pPolygon->m_rotate_recognizer = recognizer;
                return pPolygon->OnTouchEvent(enum_rotate);
            }
        }
        else
        {
            if(pPolygon->m_rotate_recognizer != NULL)
            {
                return ((PolygonTouchResponder*)Polygons[i])->OnTouchEvent(enum_rotate);
            }
        }
    }
}

void TouchMsgCenter::pinching(UIPinchGestureRecognizer* recognizer, point_2d point1, point_2d point2)
{
    bool event_began = !
    (point1.x == invalid_value &&
     point1.y == invalid_value &&
     point2.x == invalid_value &&
     point2.y == invalid_value);
    
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(event_began == true)
        {
            if(pPolygon->IsPointWithinThisArea(point1) &&
               pPolygon->IsPointWithinThisArea(point2))
            {
                pPolygon->m_pinching = true;
                pPolygon->m_pinch_recognizer = recognizer;
                return pPolygon->OnTouchEvent(enum_pinch);
            }
        }
        else
        {
            if(pPolygon->m_pinch_recognizer != NULL)
            {
                return ((PolygonTouchResponder*)Polygons[i])->OnTouchEvent(enum_pinch);
            }
        }
    }
}

void TouchMsgCenter::two_finger_double_click(point_2d point1, point_2d point2)
{
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(pPolygon->IsPointWithinThisArea(point1) &&
           pPolygon->IsPointWithinThisArea(point2))
        {
            return pPolygon->OnTouchEvent(enum_two_finger_double_click);
        }
    }
}

void TouchMsgCenter::two_finger_single_click(point_2d point1, point_2d point2)
{
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(pPolygon->IsPointWithinThisArea(point1) &&
           pPolygon->IsPointWithinThisArea(point2))
        {
            return pPolygon->OnTouchEvent(enum_two_finger_single_click);
        }
    }
}

void TouchMsgCenter::single_finger_double_click(point_2d point)
{
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(pPolygon->IsPointWithinThisArea(point))
        {
            return pPolygon->OnTouchEvent(enum_single_finger_double_click);
        }
    }
}

void TouchMsgCenter::long_press_began(point_2d point)
{
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(pPolygon->IsPointWithinThisArea(point))
        {
            return pPolygon->OnTouchEvent(enum_long_press_began);
        }
    }
}

void TouchMsgCenter::long_press_end(point_2d point)
{
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(pPolygon->IsPointWithinThisArea(point))
        {
            return pPolygon->OnTouchEvent(enum_long_press_end);
        }
    }
}

void TouchMsgCenter::single_finger_single_click(point_2d point)
{
    [MainView endEditing:YES];
    for(int i = 0; i < Polygons.GetLength(); ++i)
    {
        PolygonTouchResponder* pPolygon = (PolygonTouchResponder*)Polygons[i];
        if(pPolygon->IsPointWithinThisArea(point))
        {
            return pPolygon->OnTouchEvent(enum_single_finger_single_click);
        }
    }
}

void TouchMsgCenter::RemoveEventListener(PolygonTouchResponder* pResponder)
{
    if(pResponder == nullptr)return;
    
    Polygons.DeleteObject(pResponder);
}

void TouchMsgCenter::AddEventListener(PolygonTouchResponder* pResponder)
{
    if(pResponder == nullptr)return;

    Polygons.PushAtHead(pResponder);
}
//
//  TouchMsgCenter.h
//  Simple Books
//
//  Created by Allen Zhao on 5/15/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__TouchMsgCenter__
#define __Simple_Books__TouchMsgCenter__

#include "PolygonTouchResponder.h"
#include "DataStructure.h"
#import "Math.h"

struct os_package
{
    UITouch* touch;
    unsigned Type;
};

class PolygonTouchResponder;
class TouchMsgCenter
{
    Deque Polygons;

public:
    TouchMsgCenter();
    ~TouchMsgCenter();
    
    void AddEventListener(PolygonTouchResponder* pResponder);
    void RemoveEventListener(PolygonTouchResponder* pResponder);
    
    void os_touch_event single_finger_single_click(point_2d);
    void os_touch_event single_finger_double_click(point_2d);
    void os_touch_event two_finger_single_click(point_2d, point_2d);
    void os_touch_event two_finger_double_click(point_2d, point_2d);
    void os_touch_event long_press_began(point_2d);
    void os_touch_event long_press_end(point_2d);
    void os_touch_event pinching(
                                 UIPinchGestureRecognizer*,
                                 point_2d p1 = MakePoint2D((float)invalid_value, (float)invalid_value),
                                 point_2d p2 = MakePoint2D((float)invalid_value, (float)invalid_value));
    void os_touch_event rotating(
                                 UIRotationGestureRecognizer*,
                                 point_2d p1 = MakePoint2D((float)invalid_value, (float)invalid_value),
                                 point_2d p2 = MakePoint2D((float)invalid_value, (float)invalid_value));
    void os_touch_event pan(
                                 UIPanGestureRecognizer*,
                                 point_2d p = MakePoint2D((float)invalid_value, (float)
                                     invalid_value));
    void os_touch_event double_pan(
                                 UIPanGestureRecognizer*,
                                   point_2d p1 = MakePoint2D((float)invalid_value, (float)invalid_value),
                                   point_2d p2 = MakePoint2D((float)invalid_value, (float)invalid_value));
    void os_touch_event direct_event(os_package);
};

extern TouchMsgCenter MsgCenter;
#endif /* defined(__Simple_Books__TouchMsgCenter__) */

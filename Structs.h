//
//  Structs.h
//  Simple Books
//
//  Created by Allen Zhao on 8/3/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__Structs__
#define __Simple_Books__Structs__

#include <stdio.h>
#import "ViewController.h"

struct TouchSpeedSignPackage
{
    UITouch* event;
    bool     m_enable_increase_speed;
    double   time;
};

#endif /* defined(__Simple_Books__Structs__) */

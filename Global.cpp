//
//  Global.cpp
//  Simple Books
//
//  Created by Allen Zhao on 6/16/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include <stdio.h>
#import "ViewController.h"

float AspectRatio = 0.0f;
unsigned sizeof_addr = sizeof(void*);

void DropFrame()
{
    VC.preferredFramesPerSecond = 30;
}

void IncreaseFrame()
{
    VC.preferredFramesPerSecond = 60;
}
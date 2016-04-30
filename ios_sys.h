//
//  ios_sys.h
//  Simple Books
//
//  Created by Allen Zhao on 5/2/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__ios_sys__
#define __Simple_Books__ios_sys__

#import <GLKit/GLKit.h>
#include "macro.h"
#import  "Math.h"

namespace ios_sys {
    static UIColor* TyrantGold = [UIColor colorWithRed:1.0f green:149.0f/255.0f blue:3.0f/255.0f alpha:100.0f];
    static UIColor* black = [UIColor blackColor];
    static UIColor* darkGray = [UIColor darkGrayColor];
    static UIColor* lightGray = [UIColor lightGrayColor];
    static UIColor* gray = [UIColor grayColor];
    static UIColor* red = [UIColor redColor];
    static UIColor* green = [UIColor greenColor];
    static UIColor* blue = [UIColor blueColor];
    static UIColor* cyan = [UIColor cyanColor];
    static UIColor* yellow = [UIColor yellowColor];
    static UIColor* magenta = [UIColor magentaColor];
    static UIColor* orange = [UIColor orangeColor];
    static UIColor* purple = [UIColor purpleColor];
    static UIColor* brown = [UIColor brownColor];
    static UIColor* clear = [UIColor clearColor];
    
      static point_2d gl_bound = MakePoint2D([[UIScreen mainScreen] bounds].size.width / [[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height / [[UIScreen mainScreen] bounds].size.width);
      static const float circle_base_v = 0.0002f;
    
      float gl_to_logic(float);
      float logic_to_gl(float);
    
      point_2d logic_coordinate_to_gl_coordinate(point_2d Point);
      point_2d gl_coordinate_to_logic_coordinate(point_2d Point);
    
      double GetLocalSysTime();
      NSDate* GetLocalSysTimeNSDate();
    
      double GetUTCSysTime();
      NSDate* GetUTCSysTimeNSDate();
    
      UIColor* ColorWithRGBA(float, float, float, float);
      NSString* GetCurrentDateBlock();
      NSString* TransferDateBlock(NSDate*);
    unsigned TransferDateDay(NSDate* Date);
    NSDate* UTCTimeToLocal(NSDate*);
    NSDate* LocalTimeToUTC(NSDate* Date);
    NSDate* GetAfterDays(NSDate*, int);
    NSDate* FormatTime(const char*);
    
    NSDate* GetNextDay(NSDate*);
    NSDate* GetPreviousDay(NSDate*);
    NSDate* GetNextWeek(NSDate*);
    NSDate* GetPreviousWeek(NSDate*);
    NSDate* GetNextMonth(NSDate*);
    NSDate* GetPreviousMonth(NSDate*);
    NSDate* GetNextQuarter(NSDate*);
    NSDate* GetPreviousQuarter(NSDate*);
    NSDate* GetNextYear(NSDate*);
    NSDate* GetPreviousYear(NSDate*);
}

#endif /* defined(__Simple_Books__ios_sys__) */

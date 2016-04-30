//
//  CommonFunctions.h
//  Simple Books
//
//  Created by Allen Zhao on 8/26/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__CommonFunctions__
#define __Simple_Books__CommonFunctions__

#include <stdio.h>
#import <GLKit/GLKit.h>

NSString* GetCurrentYear();
NSString* GetCurrentQuarter();
NSString* GetCurrentMonth();
NSString* GetCurrentWeek();
NSString* GetCurrentDay();

NSString* DayFormat(NSDate*);
NSString* WeekFormat(NSDate*);
NSString* MonthFormat(NSDate*);
NSString* QuarterFormat(NSDate*);
NSString* YearFormat(NSDate*);
NSString* CheckViewDataFormat(unsigned, float, float);

#endif /* defined(__Simple_Books__CommonFunctions__) */

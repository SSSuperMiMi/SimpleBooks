//
//  ios_sys.cpp
//  Simple Books
//
//  Created by Allen Zhao on 5/2/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "ios_sys.h"
#include "Global.h"

float ios_sys::gl_to_logic(float x)
{
    return x * [[UIScreen mainScreen] bounds].size.width  / 2;
}

float ios_sys::logic_to_gl(float y)
{
    return y * 2 / [[UIScreen mainScreen] bounds].size.width;
}

point_2d ios_sys::logic_coordinate_to_gl_coordinate(point_2d Point)
{
    float logic_width_half  = [[UIScreen mainScreen] bounds].size.width  / 2;
    float logic_heitht_half = [[UIScreen mainScreen] bounds].size.height / 2;

    return MakePoint2D(
                       (Point.x - logic_width_half)  / logic_width_half,
                       (logic_heitht_half - Point.y) / logic_heitht_half / AspectRatio
                       );
}

point_2d ios_sys::gl_coordinate_to_logic_coordinate(point_2d Point)
{
    float logic_width_half  = [[UIScreen mainScreen] bounds].size.width  / 2;
    float logic_heitht_half = [[UIScreen mainScreen] bounds].size.height / 2;
    
    return MakePoint2D(
                       Point.x * logic_width_half + logic_width_half,
                       logic_heitht_half - Point.y * logic_heitht_half * AspectRatio
                       );
}

UIColor* ios_sys::ColorWithRGBA(float r, float g, float b, float a)
{
     return [UIColor colorWithRed:1.0f green:g blue:b alpha:a];
}

double ios_sys::GetUTCSysTime()
{
    return [GetUTCSysTimeNSDate() timeIntervalSince1970] * 1000;
}

NSDate* ios_sys::GetUTCSysTimeNSDate()
{
    return [NSDate date];
}

NSDate* ios_sys::GetLocalSysTimeNSDate()
{
    return UTCTimeToLocal([NSDate date]);
}

double ios_sys::GetLocalSysTime()
{
    return [GetLocalSysTimeNSDate() timeIntervalSince1970] * 1000;
}

NSString* ios_sys::GetCurrentDateBlock()
{
    return TransferDateBlock([NSDate date]);
}

NSString* ios_sys::TransferDateBlock(NSDate* UTCDate)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:UTCDate];
    
    return [NSString stringWithFormat:@"%d.%d\r%d", [comps month], [comps day], [comps year]];
}

unsigned ios_sys::TransferDateDay(NSDate* UTCDate)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:UTCDate];
    
    return [comps day];
}

NSDate* ios_sys::UTCTimeToLocal(NSDate* Date){
    NSTimeZone *zone = [NSTimeZone systemTimeZone];
    NSInteger interval = [zone secondsFromGMTForDate: Date];
    NSDate *localeDate = [Date  dateByAddingTimeInterval: interval];
    
    return localeDate;
}

NSDate* ios_sys::LocalTimeToUTC(NSDate* Date){
    NSTimeZone *zone = [NSTimeZone systemTimeZone];
    NSInteger interval = [zone secondsFromGMTForDate: Date];
    NSDate *localeDate = [Date  dateByAddingTimeInterval: -interval];
    
    return localeDate;
}

NSDate* ios_sys::GetAfterDays(NSDate* DateNow, int Days)
{
    return [NSDate dateWithTimeInterval:(24 * Days) * 60 * 60 sinceDate:DateNow];
}

NSDate* ios_sys::FormatTime(const char* Time)
{
    // eg: "2015-08-01 00:00:00"
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    return [dateFormatter dateFromString:[NSString stringWithUTF8String:Time]];
}

NSDate* ios_sys::GetNextDay(NSDate* date)
{
    return [NSDate dateWithTimeInterval:24 * 60 * 60 sinceDate:date];
}

NSDate* ios_sys::GetPreviousDay(NSDate* date)
{
    return [NSDate dateWithTimeInterval:-24 * 60 * 60 sinceDate:date];
}

NSDate* ios_sys::GetNextWeek(NSDate* date)
{
    return GetAfterDays(date, 7);
}

NSDate* ios_sys::GetPreviousWeek(NSDate* date)
{
    return GetAfterDays(date, -7);
}

NSDate* ios_sys::GetNextMonth(NSDate* date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:date];
    [comps setMonth:([comps month] + 1)];
    return [calendar dateFromComponents:comps];
}

NSDate* ios_sys::GetPreviousMonth(NSDate* date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:date];
    [comps setMonth:([comps month] - 1)];
    return [calendar dateFromComponents:comps];
}

NSDate* ios_sys::GetNextQuarter(NSDate* date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:date];
    [comps setMonth:([comps month] + 3)];
    return [calendar dateFromComponents:comps];
}

NSDate* ios_sys::GetPreviousQuarter(NSDate* date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:date];
    [comps setMonth:([comps month] - 3)];
    return [calendar dateFromComponents:comps];
}

NSDate* ios_sys::GetNextYear(NSDate* date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:date];
    [comps setMonth:([comps month] + 12)];
    return [calendar dateFromComponents:comps];
}

NSDate* ios_sys::GetPreviousYear(NSDate* date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:date];
    [comps setMonth:([comps month] - 12)];
    return [calendar dateFromComponents:comps];
}
















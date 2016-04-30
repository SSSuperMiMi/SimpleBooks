//
//  CommonFunctions.cpp
//  Simple Books
//
//  Created by Allen Zhao on 8/26/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "CommonFunctions.h"
#import "Math.h"
#import "ios_sys.h"
#import "Enum.h"

NSString* DayFormat(NSDate* Date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:Date];
    return [NSString stringWithFormat:@"%d.%d.%d", [comps year], [comps month], [comps day]];
}

NSString* WeekFormat(NSDate* Date)
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:Date];
    
    NSString* sToday/* utc */ = [NSString stringWithFormat:@"%ld-%ld-%ld 00:00:00", (long)[comps year], (long)[comps month], (long)[comps day]];
    NSDate* Today = [dateFormatter dateFromString:sToday];
    unsigned weekday = [comps weekday];
    
    NSDate* date1 = ios_sys::GetAfterDays(Today, 1-weekday);
    NSDate* date2 = ios_sys::GetAfterDays(Today, 7-weekday);
    
    NSDateComponents *comps1  = [calendar components:unitFlags fromDate:date1];
    NSDateComponents *comps2  = [calendar components:unitFlags fromDate:date2];
    
    return [NSString stringWithFormat:@"%d.%d.%d ~ %d.%d.%d", [comps1 year], [comps1 month], [comps1 day],
            [comps2 year], [comps2 month], [comps2 day]];
}

NSString* MonthFormat(NSDate* Date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:Date];
    
    return [NSString stringWithFormat:@"%d.%d", [comps year], [comps month]];
}

NSString* QuarterFormat(NSDate* Date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:Date];
    unsigned SM, EM;
    GetQuarterStartMonthAndEndMonth([comps month], SM, EM);
    
    return [NSString stringWithFormat:@"%d.%d ~ %d.%d", [comps year], SM, [comps year], EM];
}

NSString* YearFormat(NSDate* Date)
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:Date];
    return [NSString stringWithFormat:@"%d", [comps year]];
}

NSString* GetCurrentYear()
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:[NSDate date]];
    return [NSString stringWithFormat:@"%d", [comps year]];
}

NSString* GetCurrentQuarter()
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:[NSDate date]];
    unsigned SM, EM;
    GetQuarterStartMonthAndEndMonth([comps month], SM, EM);
    
    return [NSString stringWithFormat:@"%d.%d ~ %d.%d", [comps year], SM, [comps year], EM];
}

NSString* GetCurrentMonth()
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:[NSDate date]];
    
    return [NSString stringWithFormat:@"%d.%d", [comps year], [comps month]];
}

NSString* GetCurrentWeek()
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:[NSDate date]];
    
    NSString* sToday/* utc */ = [NSString stringWithFormat:@"%ld-%ld-%ld 00:00:00", (long)[comps year], (long)[comps month], (long)[comps day]];
    NSDate* Today = [dateFormatter dateFromString:sToday];
    unsigned weekday = [comps weekday];
    
    NSDate* date1 = ios_sys::GetAfterDays(Today, 1-weekday);
    NSDate* date2 = ios_sys::GetAfterDays(Today, 7-weekday);
    
    NSDateComponents *comps1  = [calendar components:unitFlags fromDate:date1];
    NSDateComponents *comps2  = [calendar components:unitFlags fromDate:date2];
    
    return [NSString stringWithFormat:@"%d.%d.%d ~ %d.%d.%d", [comps1 year], [comps1 month], [comps1 day],
            [comps2 year], [comps2 month], [comps2 day]];
}

NSString* GetCurrentDay()
{
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:[NSDate date]];
    return [NSString stringWithFormat:@"%d.%d.%d", [comps year], [comps month], [comps day]];
}

NSString* CheckViewDataFormat(unsigned type, float money, float budget)
{
    NSString* Title;
    switch (type) {
        case All_expense:
            Title = [NSString stringWithUTF8String:"Expense"];
            break;
            
        case expense_clothes:
            Title = [NSString stringWithUTF8String:"Clothes"];
            break;
            
        case expense_housing:
            Title = [NSString stringWithUTF8String:"Housing"];
            break;
            
        case expense_traffic:
            Title = [NSString stringWithUTF8String:"Traffic"];
            break;
            
        case expense_food:
            Title = [NSString stringWithUTF8String:"Food"];
            break;
            
        case expense_others:
            Title = [NSString stringWithUTF8String:"Others"];
            break;
            
        case All_income:
            Title = [NSString stringWithUTF8String:"Income"];
            break;
            
        case income_bonus:
            Title = [NSString stringWithUTF8String:"Bonus"];
            break;
            
        case income_salary:
            Title = [NSString stringWithUTF8String:"Salary"];
            break;
            
        case income_change:
            Title = [NSString stringWithUTF8String:"Change"];
            break;
            
        case income_investment:
            Title = [NSString stringWithUTF8String:"Investment"];
            break;
            
        case income_others:
            Title = [NSString stringWithUTF8String:"Others"];
            break;
        
        
        default:
            break;
    }
    
    return [NSString stringWithFormat:@"%@ : %g%%, %g$ of %g$.", Title, 100 * money / budget, money, budget];
}



















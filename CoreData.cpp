//
//  CoreData.cpp
//  Simple Books
//
//  Created by Allen Zhao on 8/16/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "CoreData.h"
#import "Macro.h"
#import "ViewController.h"
#import "ios_sys.h"
#import "Versios.h"

CoreData CD;
CoreData::CoreData()
{
}

CoreData::~CoreData()
{
}

NSArray* CoreData::QueryMoney(unsigned Type)
{
    NSFetchedResultsController* nsfrc = [VC GetfetchedResultsController:@"Account"  sortDescriptor:@"money" Section:@"money" Cache:@"moneycache"];
    NSManagedObjectContext* context = [nsfrc managedObjectContext];
    NSEntityDescription *entity = [[nsfrc fetchRequest] entity];
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];

    [fetchRequest setEntity:entity];
    NSError *error = nil;
    NSPredicate * qcondition= [NSPredicate predicateWithFormat:@"type = %d", Type];
    fetchRequest.predicate = qcondition;
    
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
    return [context executeFetchRequest:fetchRequest error:&error];
}

NSArray* CoreData::QueryMoney(NSDate* BeginDate, NSDate* EndDate)
{
    if (BeginDate == nullptr && EndDate == nullptr) {
        return nullptr;
    }
    
    NSFetchedResultsController* nsfrc = [VC GetfetchedResultsController:@"Account"  sortDescriptor:@"money" Section:@"money" Cache:@"moneycache"];
    NSManagedObjectContext* context = [nsfrc managedObjectContext];
    NSEntityDescription *entity = [[nsfrc fetchRequest] entity];
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];

    [fetchRequest setEntity:entity];
    NSError *error = nil;
    NSPredicate * qcondition ;
    if (BeginDate == nullptr) {
        qcondition = [NSPredicate predicateWithFormat:@"time < %@", EndDate];
    }
    else if (EndDate == nullptr) {
        qcondition = [NSPredicate predicateWithFormat:@"time >= %@", BeginDate];
    }
    else
    {
        qcondition = [NSPredicate predicateWithFormat:@"time >= %@ and time < %@", BeginDate, EndDate];
    }
    
    fetchRequest.predicate = qcondition;
    
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
    return [context executeFetchRequest:fetchRequest error:&error];
}

NSArray* CoreData::Query(const char* EntityName, const char* sortDescriptorName, const char* SectionName, const char* CacheName, const char* conditions)
{
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    NSManagedObjectContext *context = [[VC GetfetchedResultsController:[NSString stringWithUTF8String:EntityName]  sortDescriptor:[NSString stringWithUTF8String:sortDescriptorName]  Section:[NSString stringWithUTF8String:SectionName] Cache:[NSString stringWithUTF8String:CacheName]] managedObjectContext];
    NSEntityDescription* entity = [NSEntityDescription entityForName:[NSString stringWithUTF8String:EntityName] inManagedObjectContext:context];
    
    [fetchRequest setEntity:entity];
    NSError *error = nil;
    if (strcmp(conditions, "") != 0) {
        NSPredicate * qcondition= [NSPredicate predicateWithFormat:[NSString stringWithUTF8String:conditions]];
        fetchRequest.predicate = qcondition;
    }
    
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
    
    return [context executeFetchRequest:fetchRequest error:&error];
}

void CoreData::SaveMoneyToHD(NSDate* UTCDate, float Money, unsigned Type)
{
    NSFetchedResultsController* nsfrc = [VC GetfetchedResultsController:@"Account"  sortDescriptor:@"money" Section:@"money" Cache:@"moneycache"];
    NSManagedObjectContext* context = [nsfrc managedObjectContext];
    NSEntityDescription *entity = [[nsfrc fetchRequest] entity];
    NSManagedObject *Account = [NSEntityDescription insertNewObjectForEntityForName:[entity name] inManagedObjectContext:context];
    
    [Account setValue:UTCDate forKey:@"time"];
    [Account setValue:[NSNumber numberWithFloat:Money] forKey:@"money"];
    [Account setValue:[NSNumber numberWithInt:Type] forKey:@"type"];

    NSError *error = nil;
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
}

float CoreData::GetTargetTimeMoney(unsigned TimeType, unsigned MoneyType, NSDate* TargetTime)
{
    if (TimeType > period_type_day || MoneyType > All_expense) {
        return 0;
    }
    float Money = 0;
    NSDate* UTCDate = TargetTime == nullptr ? ios_sys::GetUTCSysTimeNSDate():TargetTime;
    NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSInteger unitFlags = NSCalendarUnitYear | NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekday|
    NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond;
    NSDateComponents *comps  = [calendar components:unitFlags fromDate:UTCDate];
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    
    NSDate *date1 ;
    NSDate *date2 ;
    
    if (TimeType == period_type_year) {
        NSString* s_date1 = [NSString stringWithFormat:@"%ld-01-01 00:00:00", (long)[comps year]];
        NSString* s_date2 = [NSString stringWithFormat:@"%ld-01-01 00:00:00", (long)([comps year] + 1)];
        
        date1 = [dateFormatter dateFromString:s_date1];
        date2 = [dateFormatter dateFromString:s_date2];
    }

    if (TimeType == period_type_quarter) {
        unsigned StartM;
        unsigned EndM;
        GetQuarterStartMonthAndEndMonth([comps month], StartM, EndM);
        
        NSString* s_date1 = [NSString stringWithFormat:@"%ld-%u-01 00:00:00", (long)[comps year], StartM];
        NSString* s_date2 = [NSString stringWithFormat:@"%ld-%u-01 00:00:00", (long)[comps year], EndM];
        
        date1 = [dateFormatter dateFromString:s_date1];
        date2 = [dateFormatter dateFromString:s_date2];
    }
    
    if (TimeType == period_type_month) {
        NSString* s_date1 = [NSString stringWithFormat:@"%ld-%ld-01 00:00:00", (long)[comps year], (long)[comps month]];
        NSString* s_date2 = [NSString stringWithFormat:@"%ld-%ld-01 00:00:00", (long)[comps year], (long)[comps month] + 1];
        
        date1 = [dateFormatter dateFromString:s_date1];
        date2 = [dateFormatter dateFromString:s_date2];
    }
    
    if (TimeType == period_type_week) {
        NSString* sToday = [NSString stringWithFormat:@"%ld-%ld-%ld 00:00:00", (long)[comps year], (long)[comps month], (long)[comps day]];
        NSDate* Today = [dateFormatter dateFromString:sToday];
        unsigned weekday = [comps weekday];
        
        date1 = ios_sys::GetAfterDays(Today, 1-weekday);
        date2 = ios_sys::GetAfterDays(Today, 8-weekday);
    }
    
    if (TimeType == period_type_day) {
        NSString* sToday = [NSString stringWithFormat:@"%ld-%ld-%ld 00:00:00", (long)[comps year], (long)[comps month], (long)[comps day]];
        NSDate* Today = [dateFormatter dateFromString:sToday];
        date1 = Today;
        date2 = ios_sys::GetAfterDays(Today, 1);
    }
    
    NSArray *fetchedObjects = CD.QueryMoney(date1, date2);
    for (NSManagedObject *info in fetchedObjects) {
        float _money = [[info valueForKey:@"money"] floatValue];
        unsigned _type = [[info valueForKey:@"type"] integerValue];
        
        if (MoneyType == All_income) {
            if (_type >= income_salary && _type <= income_others) {
                Money += _money;
            }
        }
        else if (MoneyType == All_expense)
        {
            if (_type >= expense_clothes && _type <= expense_others) {
                Money += _money;
            }
        }
        else
        {
            if (_type == MoneyType) {
                Money += _money;
            }
        }
    }
    
    return Money;
}

void CoreData::DataTest()
{
//    // 2015.8.27测试，全部通过
//    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
//    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
//
//    NSDate* time = [dateFormatter dateFromString:@"2015-08-27 18:14:00"];
//    CD.SaveMoneyToHD(time, 200, expense_housing);
//    
//    time = [dateFormatter dateFromString:@"2015-08-25 00:00:00"];
//    CD.SaveMoneyToHD(time, 50, expense_food);
//    
//    time = [dateFormatter dateFromString:@"2015-08-24 00:00:00"];
//    CD.SaveMoneyToHD(time, 16, expense_food);
//    
//    time = [dateFormatter dateFromString:@"2015-08-23 00:00:00"];
//    CD.SaveMoneyToHD(time, 19.37, expense_others);
//    
//    time = [dateFormatter dateFromString:@"2015-08-22 00:00:00"];
//    CD.SaveMoneyToHD(time, 24.51, expense_others);
//    
//    time = [dateFormatter dateFromString:@"2015-08-22 00:00:00"];
//    CD.SaveMoneyToHD(time, 26, expense_food);
//    
//    time = [dateFormatter dateFromString:@"2015-08-20 00:00:00"];
//    CD.SaveMoneyToHD(time, 21, expense_food);
//    
//    time = [dateFormatter dateFromString:@"2015-08-19 00:00:00"];
//    CD.SaveMoneyToHD(time, 285 * 3, expense_traffic);
//    
//    time = [dateFormatter dateFromString:@"2015-08-17 00:00:00"];
//    CD.SaveMoneyToHD(time, 0.7, expense_food);
//    
//    time = [dateFormatter dateFromString:@"2015-08-16 00:00:00"];
//    CD.SaveMoneyToHD(time, 13.16, expense_food);
//    
//    time = [dateFormatter dateFromString:@"2015-08-15 00:00:00"];
//    CD.SaveMoneyToHD(time, 50, expense_others);
//    // 2015.8.27测试，全部通过
}

void CoreData::CDTest()
{
//    //
//    //    Test begin.
//    //    2015.8.22
//    //    测试数据组如下:  测试全部通过.
//    //
//    //    2014.3.20       salary    30;
//    //    2014.3.23       food      50;
//    //    2015.4.9        food      70;
//    //    2015.4.10       bonus     60;
//    //    2015.5.23       clothers  300;
//    //    2015.7.30       traffic   300;
//    //    2015.8.1        food      200; // 星期六.
//    //    2015.8.1        clothers  150;
//    //    2015.8.1        salary    140;
//    //    2015.8.1        bonus     170;
//    //    2015.8.2        change    15;
//         NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
//        [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
//        
//        NSDate* _81 = [dateFormatter dateFromString:@"2015-08-01 00:00:00"];
//        
//        float _81f = GetTargetTimeMoney(period_type_day, expense_food, _81); // 200
//        float _81s = GetTargetTimeMoney(period_type_day, income_salary, _81);// 140
//        float _81income = GetTargetTimeMoney(period_type_day, All_income, _81);// 310
//        float _81expense = GetTargetTimeMoney(period_type_day, All_expense, _81);// 350
//        float todayincome = GetTargetTimeMoney(period_type_day, All_income); // 0
//        float _81weektraffic = GetTargetTimeMoney(period_type_week, expense_traffic, _81);//300
//        float _81weekbonus = GetTargetTimeMoney(period_type_week, income_bonus, _81);//170
//        float _81weekincome = GetTargetTimeMoney(period_type_week, All_income, _81);//310
//        float _81weekexpense = GetTargetTimeMoney(period_type_week, All_expense, _81);//650
//        
//        NSDate* _49 = [dateFormatter dateFromString:@"2015-04-09 00:00:00"];
//        float _49quarterfood = GetTargetTimeMoney(period_type_quarter, expense_food, _49);//70
//        float _49quarterincome = GetTargetTimeMoney(period_type_quarter, All_income, _49);//60
//        float _49quarterexpense = GetTargetTimeMoney(period_type_quarter, All_expense, _49);//370
//        float _49monthincome = GetTargetTimeMoney(period_type_month, All_income, _49);//60
//        float _49monthexpense = GetTargetTimeMoney(period_type_month, All_expense, _49);//70
//        float _2015income = GetTargetTimeMoney(period_type_year, All_income);//385
//        float _2015expense = GetTargetTimeMoney(period_type_year, All_expense);//1020
//    
//    
//    
//        float _2015_8_expense = GetTargetTimeMoney(period_type_month, All_expense);
//        float _2015_8_income = GetTargetTimeMoney(period_type_month, All_income);
//    
//        float _2015_8_26_expense = GetTargetTimeMoney(period_type_day, All_expense);
//    float _2015_8_26_income = GetTargetTimeMoney(period_type_day, All_income);
//    // 测试通过
//    
////        NSDate* _2015_8_21a = [dateFormatter dateFromString:@"2015-08-21 00:00:00"];
////        float a1 = GetTargetTimeMoney(period_type_day, expense_clothes, _2015_8_21a);
////        
////        NSDate* _2015_8_21b = [dateFormatter dateFromString:@"2015-08-21 23:59:59"];
////        float a2 = GetTargetTimeMoney(period_type_day, expense_clothes, _2015_8_21b);
////        
////        NSDate* _2015_8_20 = [dateFormatter dateFromString:@"2015-08-20 23:59:59"];
////        float a3 = GetTargetTimeMoney(period_type_day, expense_clothes, _2015_8_20);
////        
////        NSDate* _2015_8_22 = [dateFormatter dateFromString:@"2015-08-22 00:00:00"];
////        float a4 = GetTargetTimeMoney(period_type_day, expense_clothes, _2015_8_22);
//    //    // 测试通过
//    //
//    //    2015.8.21
//    //    Test end.
//    //
//    
//    float yearexpense = CD.GetBudget(_budget_year_expense);
//    float monthexpense = CD.GetBudget(_budget_month_expense);
//    CD.UpdateBudget(_budget_year_expense, 40000);
//    yearexpense = CD.GetBudget(_budget_year_expense);
//    
//    unsigned logincount = CD.GetLoginCount();
//    return;
//    // 2015.8.22测试通过.
}

float CoreData::GetBudget(unsigned type)
{
    NSFetchedResultsController* nsfrc = [VC GetfetchedResultsController:@"Budget"  sortDescriptor:@"dayincome" Section:@"dayincome" Cache:@"dayincomecache"];
    NSManagedObjectContext* context = [nsfrc managedObjectContext];
    NSEntityDescription *entity = [[nsfrc fetchRequest] entity];
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    
    [fetchRequest setEntity:entity];
    NSError *error = nil;

    NSArray *fetchedObjects = [context executeFetchRequest:fetchRequest error:&error];
    NSString* Key;
    switch (type) {
        case _budget_day_income:
            Key = [NSString stringWithUTF8String:"dayincome"];
            break;
        case _budget_day_expense:
            Key = [NSString stringWithUTF8String:"dayexpense"];
            break;
        case _budget_week_income:
            Key = [NSString stringWithUTF8String:"weekincome"];
            break;
        case _budget_week_expense:
            Key = [NSString stringWithUTF8String:"weekexpense"];
            break;
        case _budget_month_income:
            Key = [NSString stringWithUTF8String:"monthincome"];
            break;
        case _budget_month_expense:
            Key = [NSString stringWithUTF8String:"monthexpense"];
            break;
        case _budget_quarter_income:
            Key = [NSString stringWithUTF8String:"quarterincome"];
            break;
        case _budget_quarter_expense:
            Key = [NSString stringWithUTF8String:"quarterexpense"];
            break;
        case _budget_year_income:
            Key = [NSString stringWithUTF8String:"yearincome"];
            break;
        case _budget_year_expense:
            Key = [NSString stringWithUTF8String:"yearexpense"];
            break;
            
        default:
            break;
    }
    float money = 0;
    for (NSManagedObject *info in fetchedObjects) {
        float _money = [[info valueForKey:Key] floatValue];
        if (_money != 0) {
            money =  _money;
        }
    }
    
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
    return money;
}

void CoreData::UpdateBudget(unsigned type, float money)
{
    NSFetchedResultsController* nsfrc = [VC GetfetchedResultsController:@"Budget"  sortDescriptor:@"dayincome" Section:@"dayincome" Cache:@"dayincomecache"];
    NSManagedObjectContext* context = [nsfrc managedObjectContext];
    NSEntityDescription *entity = [[nsfrc fetchRequest] entity];
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    
    [fetchRequest setEntity:entity];
    NSError *error = nil;
    
    NSArray *fetchedObjects = [context executeFetchRequest:fetchRequest error:&error];
    NSString* Key;
    switch (type) {
        case _budget_day_income:
            Key = [NSString stringWithUTF8String:"dayincome"];
            break;
        case _budget_day_expense:
            Key = [NSString stringWithUTF8String:"dayexpense"];
            break;
        case _budget_week_income:
            Key = [NSString stringWithUTF8String:"weekincome"];
            break;
        case _budget_week_expense:
            Key = [NSString stringWithUTF8String:"weekexpense"];
            break;
        case _budget_month_income:
            Key = [NSString stringWithUTF8String:"monthincome"];
            break;
        case _budget_month_expense:
            Key = [NSString stringWithUTF8String:"monthexpense"];
            break;
        case _budget_quarter_income:
            Key = [NSString stringWithUTF8String:"quarterincome"];
            break;
        case _budget_quarter_expense:
            Key = [NSString stringWithUTF8String:"quarterexpense"];
            break;
        case _budget_year_income:
            Key = [NSString stringWithUTF8String:"yearincome"];
            break;
        case _budget_year_expense:
            Key = [NSString stringWithUTF8String:"yearexpense"];
            break;
            
        default:
            break;
    }
    for (NSManagedObject *info in fetchedObjects) {
        float _money = [[info valueForKey:Key] floatValue];
        if (_money != 0) {
            [info setValue:[NSNumber numberWithFloat:money] forKey:Key];
        }
    }
    
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
}

int CoreData::GetLoginCount()
{
    int ret = 0;
    
    NSFetchedResultsController* nsfrc = [VC GetfetchedResultsController:@"Login"  sortDescriptor:@"logincount" Section:@"logincount" Cache:@"logincache"];
    NSManagedObjectContext* context = [nsfrc managedObjectContext];
    NSEntityDescription *entity = [[nsfrc fetchRequest] entity];
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    
    [fetchRequest setEntity:entity];
    NSError *error = nil;
    
    NSArray *fetchedObjects = [context executeFetchRequest:fetchRequest error:&error];
    NSString* Key = [NSString stringWithUTF8String:"logincount"];
    
    for (NSManagedObject *info in fetchedObjects) {
        ret = [[info valueForKey:Key] integerValue];
    }
    
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
    
    return ret;
}

void CoreData::UpdateLoginCount()
{
    NSFetchedResultsController* nsfrc = [VC GetfetchedResultsController:@"Login"  sortDescriptor:@"logincount" Section:@"logincount" Cache:@"logincache"];
    NSManagedObjectContext* context = [nsfrc managedObjectContext];
    NSEntityDescription *entity = [[nsfrc fetchRequest] entity];
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
    
    [fetchRequest setEntity:entity];
    NSError *error = nil;
    
    NSArray *fetchedObjects = [context executeFetchRequest:fetchRequest error:&error];
    NSString* Key = [NSString stringWithUTF8String:"logincount"];

    for (NSManagedObject *info in fetchedObjects) {
        int logincount = [[info valueForKey:Key] integerValue];
        ++logincount;
        [info setValue:[NSNumber numberWithInteger:logincount] forKey:Key];
    }
    
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
}

void CoreData::SetInitialLoginRecord()
{
    NSFetchedResultsController* nsfrc = [VC GetfetchedResultsController:@"Login"  sortDescriptor:@"logincount" Section:@"logincount" Cache:@"logincache"];
    NSManagedObjectContext* context = [nsfrc managedObjectContext];
    NSEntityDescription *entity = [[nsfrc fetchRequest] entity];
    NSManagedObject *Account = [NSEntityDescription insertNewObjectForEntityForName:[entity name] inManagedObjectContext:context];
    
    [Account setValue:[NSNumber numberWithInt:1] forKey:@"logincount"];
    
    NSError *error = nil;
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
}

void CoreData::SetInitialBudget(unsigned type, float money)
{
    if (type > _budget_year_expense) {
        return;
    }
    
    NSFetchedResultsController* nsfrc = [VC GetfetchedResultsController:@"Budget"  sortDescriptor:@"dayincome" Section:@"dayincome" Cache:@"dayincomecache"];
    NSManagedObjectContext* context = [nsfrc managedObjectContext];
    NSEntityDescription *entity = [[nsfrc fetchRequest] entity];
    NSManagedObject *Budget = [NSEntityDescription insertNewObjectForEntityForName:[entity name] inManagedObjectContext:context];
    NSString* Key;
    switch (type) {
        case _budget_day_income:
            Key = [NSString stringWithUTF8String:"dayincome"];
            break;
        case _budget_day_expense:
            Key = [NSString stringWithUTF8String:"dayexpense"];
            break;
        case _budget_week_income:
            Key = [NSString stringWithUTF8String:"weekincome"];
            break;
        case _budget_week_expense:
            Key = [NSString stringWithUTF8String:"weekexpense"];
            break;
        case _budget_month_income:
            Key = [NSString stringWithUTF8String:"monthincome"];
            break;
        case _budget_month_expense:
            Key = [NSString stringWithUTF8String:"monthexpense"];
            break;
        case _budget_quarter_income:
            Key = [NSString stringWithUTF8String:"quarterincome"];
            break;
        case _budget_quarter_expense:
            Key = [NSString stringWithUTF8String:"quarterexpense"];
            break;
        case _budget_year_income:
            Key = [NSString stringWithUTF8String:"yearincome"];
            break;
        case _budget_year_expense:
            Key = [NSString stringWithUTF8String:"yearexpense"];
            break;
            
        default:
            break;
    }
    [Budget setValue:[NSNumber numberWithFloat:money] forKey:Key];
    
    NSError *error = nil;
    if (![context save:&error]) {
        // Replace this implementation with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
}

void CoreData::BudgetInit()
{
    //
    // 默认预算设置.
    //
    CD.SetInitialBudget(_budget_day_income, default_budget_day_income);
    CD.SetInitialBudget(_budget_day_expense, default_budget_day_expense);
    CD.SetInitialBudget(_budget_week_income, default_budget_week_income);
    CD.SetInitialBudget(_budget_week_expense, default_budget_week_expense);
    CD.SetInitialBudget(_budget_month_income, default_budget_month_income);
    CD.SetInitialBudget(_budget_month_expense, default_budget_month_expense);
    CD.SetInitialBudget(_budget_quarter_income, default_budget_quarter_income);
    CD.SetInitialBudget(_budget_quarter_expense, default_budget_quarter_expense);
    CD.SetInitialBudget(_budget_year_income, default_budget_year_income);
    CD.SetInitialBudget(_budget_year_expense, default_budget_year_expense);
}

void CoreData::LoginInitialization()
{
    UpdateLoginCount();
}

void CoreData::InstallationInitialization()
{
    BudgetInit();
    SetInitialLoginRecord();
#ifdef CoreDateTest
    DataTest();
#endif
    
}























//
//  CoreData.h
//  Simple Books
//
//  Created by Allen Zhao on 8/16/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__CoreData__
#define __Simple_Books__CoreData__

#include <stdio.h>
#import <CoreData/CoreData.h>

class CoreData
{
public:
    CoreData();
    ~CoreData();
    
    void SaveMoneyToHD(NSDate* UTCDate, float Money, unsigned Type);
    void CDTest();
    void DataTest();
    void InstallationInitialization();
    void LoginInitialization();
    void BudgetInit();
    void SetInitialBudget(unsigned, float);
    void SetInitialLoginRecord();
    void UpdateLoginCount();
    void UpdateBudget(unsigned, float);
    int GetLoginCount();
    
    float GetBudget(unsigned);
    
    NSArray* Query(const char*, const char*, const char*, const char*, const char*);
    NSArray* QueryMoney(NSDate* BeginDate = nullptr, NSDate* EndDate = nullptr);
    NSArray* QueryMoney(unsigned);
    
    float GetTargetTimeMoney(unsigned, unsigned, NSDate* Target = nullptr);
};

extern CoreData CD;

#endif /* defined(__Simple_Books__CoreData__) */

//
//  SimpleBooks.h
//  Simple Books
//
//  Created by Allen Zhao on 7/13/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__SimpleBooks__
#define __Simple_Books__SimpleBooks__

#include <stdio.h>
#include "SpecialPolygon.h"

class SimpleBooks
{
public:
    SimpleBooks();
    ~SimpleBooks();

    void Pause();
    void Resume();
    
    void SetCircle(ButtonCricle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba);
    void SetCircle(TimeCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba);
    void SetCircle(SettingsCricle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba);
    void SetCircle(EditCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba);
    void SetCircle(ExpenseCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba);
    void SetCircle(IncomeCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba);
    void SetCircle(IESwitchCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba);
    void SetSiriNote(
                     float rad,
                     float r_begin,
                     float w_end,
                     unsigned v_num);
    void Init();
    void Update();
    void UpdateTimeBlock(NSDate*);
    void InstallationInitialization();
    void LoginInitialization();
    void DetachAllGraphicsProcessingAndGyro();
    void RepulsionPuncture(TimeCircle*);
    void CheckViewVisibleSwitch(TimeCircle*);
    void UpdateDatasLabels();
    void UpdateCheckDatas(bool);
    
    NotePolygon* siri_note;
    
    TimeCircle* year;
    TimeCircle* quarter;
    TimeCircle* month;
    TimeCircle* week;
    TimeCircle* day;
    TimeCircle* Save;
    TimeCircle* Cancel;
    TimeCircle* Date;
    
    ButtonCricle* Next;
    ButtonCricle* Previous;
    
    EditCircle* Money;
    ExpenseCircle* expensecircle;
    IncomeCircle* incomecircle;
    IESwitchCircle* ieSwitchCircle;
    SettingsCricle* Settings;
    
    NSDate* m_CurrentTime;
    NSDate* m_CurrentRecodeTime;
    // CoreData
    NSDate* m_CurrentBillTime;
    UILabel* m_CheckPageTime;
    
    float   m_CurrentBillMoney;
    unsigned m_CurrentBillType;
    bool m_HasBeenSetTimeManually;
    static bool FirstTimeRun;
    point_2d m_TheSelectedTimeDefaultCircleDockPos;
    
    // 添加状态变量.
    unsigned m_AppState;
    
    UILabel* m_CheckPageExpenseAll;
    UILabel* m_CheckPageExpenseFood;
    UILabel* m_CheckPageExpenseClothes;
    UILabel* m_CheckPageExpenseTraffic;
    UILabel* m_CheckPageExpenseHousing;
    UILabel* m_CheckPageExpenseOthers;
    
    UILabel* m_CheckPageIncomeAll;
    UILabel* m_CheckPageIncomeSalary;
    UILabel* m_CheckPageIncomeBouns;
    UILabel* m_CheckPageIncomeChange;
    UILabel* m_CheckPageIncomeInvestment;
    UILabel* m_CheckPageIncomeOthers;
    
    bool m_CheckPageIncome;
    
    void SetCountDataLabels(TimeCircle*);
    void TimeCfmEnd(void*);
    
    
    
    
    
    
    

};

extern SimpleBooks* pSimpleBooks;
#endif /* defined(__Simple_Books__SimpleBooks__) */

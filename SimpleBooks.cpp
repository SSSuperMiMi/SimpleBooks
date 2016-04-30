//
//  SimpleBooks.cpp
//  Simple Books
//
//  Created by Allen Zhao on 7/13/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "SimpleBooks.h"
#import "MemoryPool.h"
#import "CallBacks.h"
#import "ios_sys.h"
#import "Versios.h"
#import "CoreData.h"
#import "Strings.h"
#import "CommonFunctions.h"

bool SimpleBooks::FirstTimeRun = false;
SimpleBooks* pSimpleBooks = nullptr;
SimpleBooks::SimpleBooks():siri_note(nullptr), year(nullptr), quarter(nullptr), month(nullptr), week(nullptr),day(nullptr), expensecircle(nullptr), incomecircle(nullptr),Money(nullptr), Save(nullptr), Cancel(nullptr), Date(nullptr), ieSwitchCircle(nullptr), Next(nullptr), Previous(nullptr), m_CheckPageTime(nullptr), m_CheckPageExpenseAll(nullptr),m_CheckPageExpenseFood(nullptr),m_CheckPageExpenseClothes(nullptr),m_CheckPageExpenseTraffic(nullptr),m_CheckPageExpenseHousing(nullptr),m_CheckPageExpenseOthers(nullptr),m_CheckPageIncomeAll(nullptr),m_CheckPageIncomeSalary(nullptr),m_CheckPageIncomeBouns(nullptr),m_CheckPageIncomeChange(nullptr),m_CheckPageIncomeInvestment(nullptr),m_CheckPageIncomeOthers(nullptr), m_CheckPageIncome(false)
{
    Init();
}

void SimpleBooks::TimeCfmEnd(void* args)
{
    if (m_AppState == _newnote) {
        return;
    }
    CheckViewVisibleSwitch((TimeCircle*)args);
    Next->SetVisible(true);
    Previous->SetVisible(true);
    m_CheckPageTime.hidden = false;
    NSString* CheckPageTime;
    TimeCircle* pTarget;
    
    switch (pSimpleBooks->m_AppState) {
        case _oncheck_day:
            CheckPageTime = GetCurrentDay();
            pTarget = pSimpleBooks->day;
            break;
            
        case _oncheck_week:
            CheckPageTime = GetCurrentWeek();
            pTarget = pSimpleBooks->week;
            break;
            
        case _oncheck_month:
            CheckPageTime = GetCurrentMonth();
            pTarget = pSimpleBooks->month;
            break;
            
        case _oncheck_quarter:
            CheckPageTime = GetCurrentQuarter();
            pTarget = pSimpleBooks->quarter;
            break;
            
        case _oncheck_year:
            CheckPageTime = GetCurrentYear();
            pTarget = pSimpleBooks->year;
            break;
            
        default:
            break;
    }
    
    pSimpleBooks->m_CheckPageTime.text = CheckPageTime;
    bool b = pSimpleBooks->m_CheckPageIncome == true;
    
    pSimpleBooks->m_CheckPageExpenseAll.hidden = b;
    pSimpleBooks->m_CheckPageExpenseFood.hidden = b;
    pSimpleBooks->m_CheckPageExpenseClothes.hidden = b;
    pSimpleBooks->m_CheckPageExpenseTraffic.hidden = b;
    pSimpleBooks->m_CheckPageExpenseHousing.hidden = b;
    pSimpleBooks->m_CheckPageExpenseOthers.hidden = b;
    
    pSimpleBooks->m_CheckPageIncomeAll.hidden = !b;
    pSimpleBooks->m_CheckPageIncomeSalary.hidden = !b;
    pSimpleBooks->m_CheckPageIncomeBouns.hidden = !b;
    pSimpleBooks->m_CheckPageIncomeChange.hidden = !b;
    pSimpleBooks->m_CheckPageIncomeInvestment.hidden = !b;
    pSimpleBooks->m_CheckPageIncomeOthers.hidden = !b;
    
    m_CheckPageExpenseAll.text = CheckViewDataFormat(All_expense, pTarget->m_expense, pTarget->m_budget_expense);
    m_CheckPageExpenseFood.text = CheckViewDataFormat(expense_food, pTarget->m_expense_food, pTarget->m_budget_expense);
    m_CheckPageExpenseClothes.text = CheckViewDataFormat(expense_clothes, pTarget->m_expense_chothes, pTarget->m_budget_expense);
    m_CheckPageExpenseTraffic.text = CheckViewDataFormat(expense_traffic, pTarget->m_expense_traffic, pTarget->m_budget_expense);
    m_CheckPageExpenseHousing.text = CheckViewDataFormat(expense_housing, pTarget->m_expense_housing, pTarget->m_budget_expense);
    m_CheckPageExpenseOthers.text = CheckViewDataFormat(expense_others, pTarget->m_expense_others, pTarget->m_budget_expense);
    
    m_CheckPageIncomeAll.text = CheckViewDataFormat(All_income, pTarget->m_income, pTarget->m_budget_expense);
    m_CheckPageIncomeSalary.text = CheckViewDataFormat(income_salary, pTarget->m_income_salary, pTarget->m_budget_expense);
    m_CheckPageIncomeBouns.text = CheckViewDataFormat(income_bonus, pTarget->m_income_bonus, pTarget->m_budget_expense);
    m_CheckPageIncomeChange.text = CheckViewDataFormat(income_change, pTarget->m_income_investment, pTarget->m_budget_expense);
    m_CheckPageIncomeInvestment.text = CheckViewDataFormat(income_investment, pTarget->m_expense, pTarget->m_budget_expense);
    m_CheckPageIncomeOthers.text = CheckViewDataFormat(income_others, pTarget->m_income_others, pTarget->m_budget_expense);
}

void SimpleBooks::SetCountDataLabels(TimeCircle* p)
{
    // format.
    // m_CheckPageExpenseAll.text =
}

void SimpleBooks::Pause()
{
    // Save to memory.
}

void SimpleBooks::Resume()
{
    // Resume from memory.
}

void SimpleBooks::UpdateTimeBlock(NSDate* date)
{
    Date->time_period_name.text = ios_sys::TransferDateBlock(date);
    m_CurrentBillTime = date;
}

void SimpleBooks::Update()
{
    NSDate* time_now = ios_sys::GetUTCSysTimeNSDate();
    
    unsigned last_day = ios_sys::TransferDateDay(m_CurrentTime);
    unsigned now_day = ios_sys::TransferDateDay(time_now);
    if (last_day != now_day) {
        Date->time_period_name.text = ios_sys::TransferDateBlock(time_now);
    }
    m_CurrentTime = time_now;
    if (m_HasBeenSetTimeManually == false) {
        m_CurrentBillTime = m_CurrentTime;
    }
}

void SimpleBooks::RepulsionPuncture(TimeCircle* Source)
{
    //
    // 引力场穿刺特效.
    //
    Array circles;

    circles.AppendData(year);
    circles.AppendData(quarter);
    circles.AppendData(month);
    circles.AppendData(week);
    circles.AppendData(day);
    circles.AppendData(Settings);
    
    for (int i = 0; i < circles.GetLength(); ++i) {
        TimeCircle* pTimeCircle = ((TimeCircle*)circles[i]);
        if (Source == pTimeCircle) {
            continue;
        }
        pTimeCircle->m_LineAcceleration = GetRepulsion(Source->GetPosition(), pTimeCircle->GetPosition(), GravitationalConstant);
    }

    siri_note->m_LineAcceleration = GetRepulsion(Source->GetPosition(), siri_note->GetPosition(), GravitationalConstant);
    siri_note->MsgOutline->m_LineAcceleration = GetRepulsion(Source->GetPosition(), siri_note->MsgOutline->GetPosition(), GravitationalConstant);
}

void SimpleBooks::UpdateCheckDatas(bool isNext)
{
    TimeCircle* pTarget;
    if (isNext == true) {
        switch (m_AppState) {
            case _oncheck_year:
                m_CurrentRecodeTime = ios_sys::GetNextYear(m_CurrentRecodeTime);
                pTarget = year;
                break;
                
            case _oncheck_quarter:
                m_CurrentRecodeTime = ios_sys::GetNextQuarter(m_CurrentRecodeTime);
                pTarget = quarter;
                break;
                
            case _oncheck_month:
                m_CurrentRecodeTime = ios_sys::GetNextMonth(m_CurrentRecodeTime);
                pTarget = month;
                break;
                
            case _oncheck_week:
                m_CurrentRecodeTime = ios_sys::GetNextWeek(m_CurrentRecodeTime);
                pTarget = week;
                break;
                
            case _oncheck_day:
                m_CurrentRecodeTime = ios_sys::GetNextDay(m_CurrentRecodeTime);
                pTarget = day;
                break;
                
            default:
                break;
        }
    }
    else
    {
        switch (m_AppState) {
            case _oncheck_year:
                m_CurrentRecodeTime = ios_sys::GetPreviousYear(m_CurrentRecodeTime);
                pTarget = year;
                break;
                
            case _oncheck_quarter:
                m_CurrentRecodeTime = ios_sys::GetPreviousQuarter(m_CurrentRecodeTime);
                pTarget = quarter;
                break;
                
            case _oncheck_month:
                m_CurrentRecodeTime = ios_sys::GetPreviousMonth(m_CurrentRecodeTime);
                pTarget = month;
                break;
                
            case _oncheck_week:
                m_CurrentRecodeTime = ios_sys::GetPreviousWeek(m_CurrentRecodeTime);
                pTarget = week;
                break;
                
            case _oncheck_day:
                m_CurrentRecodeTime = ios_sys::GetPreviousDay(m_CurrentRecodeTime);
                pTarget = day;
                break;
                
            default:
                break;
        }
    }
    pTarget->UpdateDatasByTime(m_CurrentRecodeTime);
}

void SimpleBooks::UpdateDatasLabels()
{
    NSString* CheckPageTime;
    TimeCircle* pTarget;
    
    switch (m_AppState) {
        case _oncheck_day:
            CheckPageTime = GetCurrentDay();
            pTarget = day;
            m_CheckPageTime.text = DayFormat(m_CurrentRecodeTime);
            break;
            
        case _oncheck_week:
            CheckPageTime = GetCurrentWeek();
            pTarget = week;
            m_CheckPageTime.text = WeekFormat(m_CurrentRecodeTime);
            break;
            
        case _oncheck_month:
            CheckPageTime = GetCurrentMonth();
            pTarget = month;
            m_CheckPageTime.text = MonthFormat(m_CurrentRecodeTime);
            break;
            
        case _oncheck_quarter:
            CheckPageTime = GetCurrentQuarter();
            pTarget = quarter;
            m_CheckPageTime.text = QuarterFormat(m_CurrentRecodeTime);
            break;
            
        case _oncheck_year:
            CheckPageTime = GetCurrentYear();
            pTarget = year;
            m_CheckPageTime.text = YearFormat(m_CurrentRecodeTime);
            break;
            
        default:
            break;
    }
    
    m_CheckPageExpenseAll.text = CheckViewDataFormat(All_expense, pTarget->m_expense, pTarget->m_budget_expense);
    m_CheckPageExpenseFood.text = CheckViewDataFormat(expense_food, pTarget->m_expense_food, pTarget->m_budget_expense);
    m_CheckPageExpenseClothes.text = CheckViewDataFormat(expense_clothes, pTarget->m_expense_chothes, pTarget->m_budget_expense);
    m_CheckPageExpenseTraffic.text = CheckViewDataFormat(expense_traffic, pTarget->m_expense_traffic, pTarget->m_budget_expense);
    m_CheckPageExpenseHousing.text = CheckViewDataFormat(expense_housing, pTarget->m_expense_housing, pTarget->m_budget_expense);
    m_CheckPageExpenseOthers.text = CheckViewDataFormat(expense_others, pTarget->m_expense_others, pTarget->m_budget_expense);
    
    m_CheckPageIncomeAll.text = CheckViewDataFormat(All_income, pTarget->m_income, pTarget->m_budget_expense);
    m_CheckPageIncomeSalary.text = CheckViewDataFormat(income_salary, pTarget->m_income_salary, pTarget->m_budget_expense);
    m_CheckPageIncomeBouns.text = CheckViewDataFormat(income_bonus, pTarget->m_income_bonus, pTarget->m_budget_expense);
    m_CheckPageIncomeChange.text = CheckViewDataFormat(income_change, pTarget->m_income_investment, pTarget->m_budget_expense);
    m_CheckPageIncomeInvestment.text = CheckViewDataFormat(income_investment, pTarget->m_expense, pTarget->m_budget_expense);
    m_CheckPageIncomeOthers.text = CheckViewDataFormat(income_others, pTarget->m_income_others, pTarget->m_budget_expense);
}

void SimpleBooks::CheckViewVisibleSwitch(TimeCircle* p)
{
    Array circles;
    
    circles.AppendData(year);
    circles.AppendData(quarter);
    circles.AppendData(month);
    circles.AppendData(week);
    circles.AppendData(day);
    circles.AppendData(Settings);
    
    for (int i = 0; i < circles.GetLength(); ++i) {
        TimeCircle* pTimeCircle = ((TimeCircle*)circles[i]);
        if (p == pTimeCircle) {
            p->SetVisible(true);
            continue;
        }
        pTimeCircle->SetVisible(false);
    }
}

void SimpleBooks::DetachAllGraphicsProcessingAndGyro()
{
    siri_note->MsgOutline->DetachGPCObstacle();
    year->DetachGPC();
    month->DetachGPC();
    quarter->DetachGPC();
    week->DetachGPC();
    day->DetachGPC();
    Settings->DetachGPC();
    
    year->DetachGyroPC();
    month->DetachGyroPC();
    quarter->DetachGyroPC();
    week->DetachGyroPC();
    day->DetachGyroPC();
    Settings->DetachGyroPC();
}

void SimpleBooks::LoginInitialization()
{
    CD.LoginInitialization();
}

void SimpleBooks::InstallationInitialization()
{
    CD.InstallationInitialization();
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
    UILocalNotification *notification=[[UILocalNotification alloc] init];
    if (notification!=nil) {
        notification.fireDate = ios_sys::FormatTime("2015-08-24 18:00:00");// 改为当天的晚上6:00
        notification.repeatInterval = NSCalendarUnitDay;//循环次数，kCFCalendarUnitWeekday一周一次
        notification.timeZone=[NSTimeZone defaultTimeZone];
        notification.applicationIconBadgeNumber = 1;
        notification.soundName= UILocalNotificationDefaultSoundName;//声音，可以换成alarm.soundName = @"myMusic.caf"
        //去掉下面2行就不会弹出提示框
        notification.alertBody = [NSString stringWithUTF8String:NotifyStrings[GetRandNum(0, 5)]];//提示信息 弹出提示框
        notification.alertAction = @"打开";  //提示框按钮
        //notification.hasAction = NO; //是否显示额外的按钮，为no时alertAction消失
        // NSDictionary *infoDict = [NSDictionary dictionaryWithObject:@"someValue" forKey:@"someKey"];
        //notification.userInfo = infoDict; //添加额外的信息
        
        [[UIApplication sharedApplication] scheduleLocalNotification:notification];
    }
}

void SimpleBooks::Init()
{
    if (FirstTimeRun == true) {
        InstallationInitialization();
    }
    else
    {
        LoginInitialization();
    }
    
    SetSiriNote(3 * M_PI_2, 0.3, default_circle_thickness, default_precision_of_circle + 1);
    float color[] = {0,1,0,1};
    
    SetCircle(&year, 0.2, default_circle_thickness, default_precision_of_circle_double, color);
    year->SetPosition(MakePoint2D(-0.6, 1.0));
    year->Init(period_type_year);
    year->AttachGPC();
    year->RegisterCallback(OnYear, enum_single_finger_single_click);
    year->RegisterCallback(OnTimeCFMEnd, enum_ConstantFieldModel_end);
    
    SetCircle(&quarter, 0.2 * time_circle_scale_factor, default_circle_thickness, default_precision_of_circle_double, color);
    quarter->SetPosition(MakePoint2D(-0.2, 1.3));
    quarter->Init(period_type_quarter);
    quarter->AttachGPC();
    quarter->RegisterCallback(OnQuarter, enum_single_finger_single_click);
    quarter->RegisterCallback(OnTimeCFMEnd, enum_ConstantFieldModel_end);
    
    SetCircle(&month, 0.2 * powf(time_circle_scale_factor, 2), default_circle_thickness, default_precision_of_circle_double, color);
    month->SetPosition(MakePoint2D(0.2, 0.8));
    month->Init(period_type_month);
    month->AttachGPC();
    month->RegisterCallback(OnMonth, enum_single_finger_single_click);
    month->RegisterCallback(OnTimeCFMEnd, enum_ConstantFieldModel_end);
    
    SetCircle(&week, 0.2 * powf(time_circle_scale_factor, 3), default_circle_thickness, default_precision_of_circle_double, color);
    week->SetPosition(MakePoint2D(0.3, 0.2));
    week->Init(period_type_week);
    week->AttachGPC();
    week->RegisterCallback(OnWeek, enum_single_finger_single_click);
    week->RegisterCallback(OnTimeCFMEnd, enum_ConstantFieldModel_end);
    
    SetCircle(&day, 0.2 * powf(time_circle_scale_factor, 4), default_circle_thickness, default_precision_of_circle_double, color);
    day->SetPosition(MakePoint2D(0.4, -0.5));
    day->Init(period_type_day);
    day->AttachGPC();
    day->RegisterCallback(OnDay, enum_single_finger_single_click);
    day->RegisterCallback(OnTimeCFMEnd, enum_ConstantFieldModel_end);
    
    SetCircle(&Settings, 0.2 * powf(time_circle_scale_factor, 5), default_circle_thickness, default_precision_of_circle_double, color);
    Settings->SetPosition(MakePoint2D(-0.2, -1.0));
    Settings->Init();
    Settings->AttachGPC();
    Settings->RegisterCallback(OnSettings, enum_single_finger_single_click);
    Settings->RegisterCallback(OnTimeCFMEnd, enum_ConstantFieldModel_end);
    
    SetCircle(&Save, 0.15, default_circle_thickness, default_precision_of_circle_double, color);
    Save->SetPosition(MakePoint2D(-0.6, 1.0));
    Save->Init(_save);
    Save->DetachGyroPC();
    Save->RegisterCallback(OnSave, enum_single_finger_single_click);
    Save->SetVisible(false);
    
    SetCircle(&Cancel, 0.18, default_circle_thickness, default_precision_of_circle_double, color);
    Cancel->SetPosition(MakePoint2D(-0.5, -1.0));
    Cancel->Init(_cancel);
    Cancel->DetachGyroPC();
    Cancel->RegisterCallback(OnCancel, enum_single_finger_single_click);
    Cancel->SetVisible(false);
    
    SetCircle(&Date, 0.18, default_circle_thickness, default_precision_of_circle_double, color);
    Date->SetPosition(MakePoint2D(0.0, -1.0));
    Date->Init(_date);
    Date->DetachGyroPC();
    Date->RegisterCallback(OnDate, enum_single_finger_single_click);
    Date->SetVisible(false);
    
    SetCircle(&Money, 0.17, default_circle_thickness, default_precision_of_circle_double, color);
    Money->RegisterCallback(OnMoney, enum_single_finger_single_click);
    Money->SetPosition(MakePoint2D(0.0, 1.0));
    Money->Init();
    Money->DetachGyroPC();
    Money->SetVisible(false);
    Money->EditingNotes.delegate = VC;
    
    SetCircle(&expensecircle, 0.7, default_circle_thickness, default_precision_of_circle_double, color);
    expensecircle->EnableDirectOSTouchMsg(true);
    expensecircle->DetachEvent();
    expensecircle->Init();
    expensecircle->DetachGyroPC();
    expensecircle->SetVisible(false);
    
    SetCircle(&incomecircle, 0.7, default_circle_thickness, default_precision_of_circle_double, color);
    incomecircle->EnableDirectOSTouchMsg(true);
    incomecircle->DetachEvent();
    incomecircle->Init();
    incomecircle->DetachGyroPC();
    incomecircle->SetVisible(false);
    
    SetCircle(&ieSwitchCircle, 0.3, default_circle_thickness, default_precision_of_circle_double, color);
    ieSwitchCircle->SetPosition(MakePoint2D(0.6, 1.3));
    ieSwitchCircle->DetachEvent();
    ieSwitchCircle->Init();
    ieSwitchCircle->DetachGyroPC();
    ieSwitchCircle->SetVisible(false);
    ieSwitchCircle->RegisterCallback(OnIESwitch, enum_single_finger_single_click);
    
    m_CurrentTime = ios_sys::GetUTCSysTimeNSDate();
    m_HasBeenSetTimeManually = false;
    
#ifdef CoreDateTest
    CD.CDTest();
#endif
    
    m_TheSelectedTimeDefaultCircleDockPos.x = 0;
    m_TheSelectedTimeDefaultCircleDockPos.y = 1.3;
    
    SetCircle(&Next, 0.21, default_circle_thickness, default_precision_of_circle_double, color);
    Next->SetPosition(MakePoint2D(0.5, 1.3));
    Next->DetachEvent();
    Next->Init("Next");
    Next->RegisterCallback(OnNextPeriod, enum_single_finger_single_click);
    Next->DetachGyroPC();
    Next->SetVisible(false);
    
    SetCircle(&Previous, 0.21, default_circle_thickness, default_precision_of_circle_double, color);
    Previous->SetPosition(MakePoint2D(-0.5, 1.3));
    Previous->DetachEvent();
    Previous->Init("Previous");
    Previous->DetachGyroPC();
    Previous->RegisterCallback(OnPrevoiusPeriod, enum_single_finger_single_click);
    Previous->SetVisible(false);
    
    float h = 44, w = 320;
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, 1));
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    
    m_CheckPageTime = [UILabel new];
    m_CheckPageTime.numberOfLines = 0;
    m_CheckPageTime.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageTime.text = @"";
    
    m_CheckPageTime.textColor = ios_sys::TyrantGold;
    m_CheckPageTime.textAlignment = NSTextAlignmentCenter;
    m_CheckPageTime.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    m_CheckPageTime.hidden = true;
    
    [MainView addSubview:m_CheckPageTime];
    
    m_AppState = _mainview;
    
    h = 30;
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageExpenseAll = [UILabel new];
    m_CheckPageExpenseAll.numberOfLines = 0;
    m_CheckPageExpenseAll.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageExpenseAll.text = CheckViewDataFormat(All_expense, 0, 0);
    m_CheckPageExpenseAll.textColor = ios_sys::TyrantGold;
    m_CheckPageExpenseAll.textAlignment = NSTextAlignmentCenter;
    m_CheckPageExpenseAll.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    m_CheckPageExpenseAll.hidden = true;
    
    [MainView addSubview:m_CheckPageExpenseAll];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageExpenseFood = [UILabel new];
    m_CheckPageExpenseFood.numberOfLines = 0;
    m_CheckPageExpenseFood.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageExpenseFood.text = CheckViewDataFormat(expense_food, 0, 0);
    m_CheckPageExpenseFood.hidden = true;
    m_CheckPageExpenseFood.textColor = ios_sys::TyrantGold;
    m_CheckPageExpenseFood.textAlignment = NSTextAlignmentCenter;
    m_CheckPageExpenseFood.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageExpenseFood];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - 2 * ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageExpenseClothes = [UILabel new];
    m_CheckPageExpenseClothes.numberOfLines = 0;
    m_CheckPageExpenseClothes.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageExpenseClothes.text = CheckViewDataFormat(expense_clothes, 0, 0);
    m_CheckPageExpenseClothes.hidden = true;
    m_CheckPageExpenseClothes.textColor = ios_sys::TyrantGold;
    m_CheckPageExpenseClothes.textAlignment = NSTextAlignmentCenter;
    m_CheckPageExpenseClothes.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageExpenseClothes];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - 3 * ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageExpenseTraffic = [UILabel new];
    m_CheckPageExpenseTraffic.numberOfLines = 0;
    m_CheckPageExpenseTraffic.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageExpenseTraffic.text = CheckViewDataFormat(expense_traffic, 0, 0);
    m_CheckPageExpenseTraffic.hidden = true;
    m_CheckPageExpenseTraffic.textColor = ios_sys::TyrantGold;
    m_CheckPageExpenseTraffic.textAlignment = NSTextAlignmentCenter;
    m_CheckPageExpenseTraffic.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageExpenseTraffic];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - 4 * ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageExpenseHousing = [UILabel new];
    m_CheckPageExpenseHousing.numberOfLines = 0;
    m_CheckPageExpenseHousing.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageExpenseHousing.text = CheckViewDataFormat(expense_housing, 0, 0);
    m_CheckPageExpenseHousing.hidden = true;
    m_CheckPageExpenseHousing.textColor = ios_sys::TyrantGold;
    m_CheckPageExpenseHousing.textAlignment = NSTextAlignmentCenter;
    m_CheckPageExpenseHousing.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageExpenseHousing];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - 5 * ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageExpenseOthers = [UILabel new];
    m_CheckPageExpenseOthers.numberOfLines = 0;
    m_CheckPageExpenseOthers.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageExpenseOthers.text = CheckViewDataFormat(expense_others, 0, 0);
    m_CheckPageExpenseOthers.hidden = true;
    m_CheckPageExpenseOthers.textColor = ios_sys::TyrantGold;
    m_CheckPageExpenseOthers.textAlignment = NSTextAlignmentCenter;
    m_CheckPageExpenseOthers.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageExpenseOthers];
    
    h = 30;
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageIncomeAll = [UILabel new];
    m_CheckPageIncomeAll.numberOfLines = 0;
    m_CheckPageIncomeAll.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageIncomeAll.text = CheckViewDataFormat(All_income, 0, 0);
    m_CheckPageIncomeAll.textColor = ios_sys::TyrantGold;
    m_CheckPageIncomeAll.textAlignment = NSTextAlignmentCenter;
    m_CheckPageIncomeAll.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    m_CheckPageIncomeAll.hidden = true;
    [MainView addSubview:m_CheckPageIncomeAll];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageIncomeSalary = [UILabel new];
    m_CheckPageIncomeSalary.numberOfLines = 0;
    m_CheckPageIncomeSalary.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageIncomeSalary.text = CheckViewDataFormat(income_salary, 0, 0);
    m_CheckPageIncomeSalary.hidden = true;
    m_CheckPageIncomeSalary.textColor = ios_sys::TyrantGold;
    m_CheckPageIncomeSalary.textAlignment = NSTextAlignmentCenter;
    m_CheckPageIncomeSalary.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageIncomeSalary];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - 2 * ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageIncomeBouns = [UILabel new];
    m_CheckPageIncomeBouns.numberOfLines = 0;
    m_CheckPageIncomeBouns.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageIncomeBouns.text = CheckViewDataFormat(income_bonus, 0, 0);
    m_CheckPageIncomeBouns.hidden = true;
    m_CheckPageIncomeBouns.textColor = ios_sys::TyrantGold;
    m_CheckPageIncomeBouns.textAlignment = NSTextAlignmentCenter;
    m_CheckPageIncomeBouns.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageIncomeBouns];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - 3 * ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageIncomeChange = [UILabel new];
    m_CheckPageIncomeChange.numberOfLines = 0;
    m_CheckPageIncomeChange.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageIncomeChange.text = CheckViewDataFormat(income_change, 0, 0);
    m_CheckPageIncomeChange.hidden = true;
    m_CheckPageIncomeChange.textColor = ios_sys::TyrantGold;
    m_CheckPageIncomeChange.textAlignment = NSTextAlignmentCenter;
    m_CheckPageIncomeChange.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageIncomeChange];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - 4 * ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageIncomeInvestment = [UILabel new];
    m_CheckPageIncomeInvestment.numberOfLines = 0;
    m_CheckPageIncomeInvestment.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageIncomeInvestment.text = CheckViewDataFormat(income_investment, 0, 0);
    m_CheckPageIncomeInvestment.hidden = true;
    m_CheckPageIncomeInvestment.textColor = ios_sys::TyrantGold;
    m_CheckPageIncomeInvestment.textAlignment = NSTextAlignmentCenter;
    m_CheckPageIncomeInvestment.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageIncomeInvestment];
    
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MakePoint2D(0, -0.7 - 5 * ios_sys::logic_to_gl(h)));
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    m_CheckPageIncomeOthers = [UILabel new];
    m_CheckPageIncomeOthers.numberOfLines = 0;
    m_CheckPageIncomeOthers.lineBreakMode = NSLineBreakByWordWrapping;
    m_CheckPageIncomeOthers.text = CheckViewDataFormat(income_others, 0, 0);
    m_CheckPageIncomeOthers.hidden = true;
    m_CheckPageIncomeOthers.textColor = ios_sys::TyrantGold;
    m_CheckPageIncomeOthers.textAlignment = NSTextAlignmentCenter;
    m_CheckPageIncomeOthers.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:m_CheckPageIncomeOthers];
    
    m_CurrentRecodeTime = m_CurrentTime;
}

void SimpleBooks::SetCircle(EditCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba)
{
    if (innerdiameter <= 0 || thickness <= 0 || v_num == 0 || rgba == nullptr) {
        *pCircle = nullptr;
        
        return;
    }
    float r = rgba[0];
    float g = rgba[1];
    float b = rgba[2];
    float a = rgba[3];
    
    float* gl_data;
    global_memory_pool.GetMemory(v_num * 7 * sizeof(float), (void**)&gl_data);
    float ProgressiveAngle = 0;
    for(int i = 0; i < v_num / 2; ++i)
    {
        gl_data[i * 14 + 0] = (innerdiameter + thickness) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 1] = (innerdiameter + thickness) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 2] = 0;
        gl_data[i * 14 + 3] = r;
        gl_data[i * 14 + 4] = g;
        gl_data[i * 14 + 5] = b;
        gl_data[i * 14 + 6] = a;
        gl_data[i * 14 + 7] = (innerdiameter) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 8] = (innerdiameter) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 9] = 0;
        gl_data[i * 14 + 10] = r;
        gl_data[i * 14 + 11] = g;
        gl_data[i * 14 + 12] = b;
        gl_data[i * 14 + 13] = a;
        
        ProgressiveAngle += M_PI * 4 / (v_num - 2);
    }
    
    EditCircle::CreatePolygonTouchResponder(gl_data, v_num, pCircle);
    (*pCircle)->SetRenderMode(GL_TRIANGLE_STRIP);
    global_memory_pool.ReleaseMemory(gl_data);
    (*pCircle)->radius = innerdiameter + thickness;
    (*pCircle)->insideRadius = innerdiameter;
}

void SimpleBooks::SetCircle(ExpenseCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba)
{
    if (innerdiameter <= 0 || thickness <= 0 || v_num == 0 || rgba == nullptr) {
        *pCircle = nullptr;
        
        return;
    }
    float r = rgba[0];
    float g = rgba[1];
    float b = rgba[2];
    float a = rgba[3];
    
    float* gl_data;
    global_memory_pool.GetMemory(v_num * 7 * sizeof(float), (void**)&gl_data);
    float ProgressiveAngle = 0;
    for(int i = 0; i < v_num / 2; ++i)
    {
        gl_data[i * 14 + 0] = (innerdiameter + thickness) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 1] = (innerdiameter + thickness) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 2] = 0;
        gl_data[i * 14 + 3] = r;
        gl_data[i * 14 + 4] = g;
        gl_data[i * 14 + 5] = b;
        gl_data[i * 14 + 6] = a;
        
        gl_data[i * 14 + 7] = (innerdiameter) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 8] = (innerdiameter) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 9] = 0;
        gl_data[i * 14 + 10] = r;
        gl_data[i * 14 + 11] = g;
        gl_data[i * 14 + 12] = b;
        gl_data[i * 14 + 13] = a;
        
        ProgressiveAngle += M_PI * 4 / (v_num - 2);
    }
    
    ExpenseCircle::CreatePolygonTouchResponder(gl_data, v_num, pCircle);
    (*pCircle)->SetRenderMode(GL_TRIANGLE_STRIP);
    global_memory_pool.ReleaseMemory(gl_data);
    (*pCircle)->radius = innerdiameter + thickness;
    (*pCircle)->insideRadius = innerdiameter;
    
    float half_thickness = thickness / 2;
    float deltaAngle = 0.4 * M_PI;
    float angle = 0.0f;
    float* gl_line_data;
    ExpenseCircle* pExpenseCircle = *pCircle;

    global_memory_pool.GetMemory(4 * 7 * sizeof(float), (void**)&gl_line_data);
    for (int i = 0; i < 5; ++i) {
        gl_line_data[0] = half_thickness * sinf(angle);
        gl_line_data[1] = -half_thickness * cosf(angle);
        gl_line_data[2] = 0;
        gl_line_data[3] = r;
        gl_line_data[4] = g;
        gl_line_data[5] = b;
        gl_line_data[6] = a;
        
        gl_line_data[7] = half_thickness * sinf(angle) + innerdiameter * cosf(angle);
        gl_line_data[8] = -half_thickness * cosf(angle) + innerdiameter * sinf(angle);
        gl_line_data[9] = 0;
        gl_line_data[10] = r;
        gl_line_data[11] = g;
        gl_line_data[12] = b;
        gl_line_data[13] = a;
        
        gl_line_data[14] = -half_thickness * sinf(angle) + innerdiameter * cosf(angle);
        gl_line_data[15] = half_thickness * cosf(angle) + innerdiameter * sinf(angle);
        gl_line_data[16] = 0;
        gl_line_data[17] = r;
        gl_line_data[18] = g;
        gl_line_data[19] = b;
        gl_line_data[20] = a;
        
        gl_line_data[21] = -half_thickness * sinf(angle);
        gl_line_data[22] = half_thickness * cosf(angle);
        gl_line_data[23] = 0;
        gl_line_data[24] = r;
        gl_line_data[25] = g;
        gl_line_data[26] = b;
        gl_line_data[27] = a;
        
        PolygonTouchResponder* p;
        PolygonTouchResponder::CreatePolygonTouchResponder(gl_line_data, 4, &p);
        p->SetRenderMode(GL_TRIANGLE_FAN);
        switch (i) {
            case 0:
                pExpenseCircle->DividingLine0 = p;
                break;
            
            case 1:
                pExpenseCircle->DividingLine1 = p;
                break;
            
            case 2:
                pExpenseCircle->DividingLine2 = p;
                break;
            
            case 3:
                pExpenseCircle->DividingLine3 = p;
                break;
            
            case 4:
                pExpenseCircle->DividingLine4 = p;
                break;
            
            default:
                break;
        }
        
        angle += deltaAngle;
    }
    global_memory_pool.ReleaseMemory(gl_line_data);
}

void SimpleBooks::SetCircle(IESwitchCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba)
{
    if (innerdiameter <= 0 || thickness <= 0 || v_num == 0 || rgba == nullptr) {
        *pCircle = nullptr;
        
        return;
    }
    float r = rgba[0];
    float g = rgba[1];
    float b = rgba[2];
    float a = rgba[3];
    
    float* gl_data;
    global_memory_pool.GetMemory(v_num * 7 * sizeof(float), (void**)&gl_data);
    float ProgressiveAngle = 0;
    for(int i = 0; i < v_num / 2; ++i)
    {
        gl_data[i * 14 + 0] = (innerdiameter + thickness) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 1] = (innerdiameter + thickness) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 2] = 0;
        gl_data[i * 14 + 3] = r;
        gl_data[i * 14 + 4] = g;
        gl_data[i * 14 + 5] = b;
        gl_data[i * 14 + 6] = a;
        
        gl_data[i * 14 + 7] = (innerdiameter) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 8] = (innerdiameter) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 9] = 0;
        gl_data[i * 14 + 10] = r;
        gl_data[i * 14 + 11] = g;
        gl_data[i * 14 + 12] = b;
        gl_data[i * 14 + 13] = a;
        
        ProgressiveAngle += M_PI * 4 / (v_num - 2);
    }
    
    IESwitchCircle::CreatePolygonTouchResponder(gl_data, v_num, pCircle);
    (*pCircle)->SetRenderMode(GL_TRIANGLE_STRIP);
    global_memory_pool.ReleaseMemory(gl_data);
    (*pCircle)->radius = innerdiameter + thickness;
    (*pCircle)->insideRadius = innerdiameter;
    
    float half_thickness = thickness / 2;
    float* gl_line_data;
    IESwitchCircle* pIESwitchCircle = *pCircle;
    
    global_memory_pool.GetMemory(4 * 7 * sizeof(float), (void**)&gl_line_data);
    gl_line_data[0] = innerdiameter * cosf(M_PI_4 * 3) + half_thickness * cosf(M_PI_4);
    gl_line_data[1] = innerdiameter * cosf(M_PI_4 * 3) - half_thickness * cosf(M_PI_4);
    gl_line_data[2] = 0;
    gl_line_data[3] = r;
    gl_line_data[4] = g;
    gl_line_data[5] = b;
    gl_line_data[6] = a;
    
    gl_line_data[7] = innerdiameter * cosf(M_PI_4) + half_thickness * cosf(M_PI_4);
    gl_line_data[8] = innerdiameter * cosf(M_PI_4) - half_thickness * cosf(M_PI_4);
    gl_line_data[9] = 0;
    gl_line_data[10] = r;
    gl_line_data[11] = g;
    gl_line_data[12] = b;
    gl_line_data[13] = a;
    
    gl_line_data[14] = innerdiameter * cosf(M_PI_4) - half_thickness * cosf(M_PI_4);
    gl_line_data[15] = innerdiameter * cosf(M_PI_4) + half_thickness * cosf(M_PI_4);
    gl_line_data[16] = 0;
    gl_line_data[17] = r;
    gl_line_data[18] = g;
    gl_line_data[19] = b;
    gl_line_data[20] = a;
    
    gl_line_data[21] = innerdiameter * cosf(M_PI_4 * 3) - half_thickness * cosf(M_PI_4);
    gl_line_data[22] = innerdiameter * cosf(M_PI_4 * 3) + half_thickness * cosf(M_PI_4);
    gl_line_data[23] = 0;
    gl_line_data[24] = r;
    gl_line_data[25] = g;
    gl_line_data[26] = b;
    gl_line_data[27] = a;
    
    PolygonTouchResponder* p;
    PolygonTouchResponder::CreatePolygonTouchResponder(gl_line_data, 4, &p);
    p->SetRenderMode(GL_TRIANGLE_FAN);
    pIESwitchCircle->DividingLine = p;
    global_memory_pool.ReleaseMemory(gl_line_data);
}

void SimpleBooks::SetCircle(IncomeCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba)
{
    if (innerdiameter <= 0 || thickness <= 0 || v_num == 0 || rgba == nullptr) {
        *pCircle = nullptr;
        
        return;
    }
    float r = rgba[0];
    float g = rgba[1];
    float b = rgba[2];
    float a = rgba[3];
    
    float* gl_data;
    global_memory_pool.GetMemory(v_num * 7 * sizeof(float), (void**)&gl_data);
    float ProgressiveAngle = 0;
    for(int i = 0; i < v_num / 2; ++i)
    {
        gl_data[i * 14 + 0] = (innerdiameter + thickness) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 1] = (innerdiameter + thickness) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 2] = 0;
        gl_data[i * 14 + 3] = r;
        gl_data[i * 14 + 4] = g;
        gl_data[i * 14 + 5] = b;
        gl_data[i * 14 + 6] = a;
        
        gl_data[i * 14 + 7] = (innerdiameter) * cosf(ProgressiveAngle);
        gl_data[i * 14 + 8] = (innerdiameter) * sinf(ProgressiveAngle);
        gl_data[i * 14 + 9] = 0;
        gl_data[i * 14 + 10] = r;
        gl_data[i * 14 + 11] = g;
        gl_data[i * 14 + 12] = b;
        gl_data[i * 14 + 13] = a;
        
        ProgressiveAngle += M_PI * 4 / (v_num - 2);
    }
    
    IncomeCircle::CreatePolygonTouchResponder(gl_data, v_num, pCircle);
    (*pCircle)->SetRenderMode(GL_TRIANGLE_STRIP);
    global_memory_pool.ReleaseMemory(gl_data);
    (*pCircle)->radius = innerdiameter + thickness;
    (*pCircle)->insideRadius = innerdiameter;
    
    float half_thickness = thickness / 2;
    float deltaAngle = 0.4 * M_PI;
    float angle = 0.0f;
    float* gl_line_data;
    IncomeCircle* pIncomeCircle = *pCircle;
    
    // 建模一定要逆时针.
    global_memory_pool.GetMemory(4 * 7 * sizeof(float), (void**)&gl_line_data);
    for (int i = 0; i < 5; ++i) {
        gl_line_data[0] = half_thickness * sinf(angle);
        gl_line_data[1] = -half_thickness * cosf(angle);
        gl_line_data[2] = 0;
        gl_line_data[3] = r;
        gl_line_data[4] = g;
        gl_line_data[5] = b;
        gl_line_data[6] = a;
        
        gl_line_data[7] = half_thickness * sinf(angle) + innerdiameter * cosf(angle);
        gl_line_data[8] = -half_thickness * cosf(angle) + innerdiameter * sinf(angle);
        gl_line_data[9] = 0;
        gl_line_data[10] = r;
        gl_line_data[11] = g;
        gl_line_data[12] = b;
        gl_line_data[13] = a;
        
        gl_line_data[14] = -half_thickness * sinf(angle) + innerdiameter * cosf(angle);
        gl_line_data[15] = half_thickness * cosf(angle) + innerdiameter * sinf(angle);
        gl_line_data[16] = 0;
        gl_line_data[17] = r;
        gl_line_data[18] = g;
        gl_line_data[19] = b;
        gl_line_data[20] = a;
        
        gl_line_data[21] = -half_thickness * sinf(angle);
        gl_line_data[22] = half_thickness * cosf(angle);
        gl_line_data[23] = 0;
        gl_line_data[24] = r;
        gl_line_data[25] = g;
        gl_line_data[26] = b;
        gl_line_data[27] = a;
        
        PolygonTouchResponder* p;
        PolygonTouchResponder::CreatePolygonTouchResponder(gl_line_data, 4, &p);
        p->SetRenderMode(GL_TRIANGLE_FAN);
        switch (i) {
            case 0:
                pIncomeCircle->DividingLine0 = p;
                break;
                
            case 1:
                pIncomeCircle->DividingLine1 = p;
                break;
                
            case 2:
                pIncomeCircle->DividingLine2 = p;
                break;
                
            case 3:
                pIncomeCircle->DividingLine3 = p;
                break;
                
            case 4:
                pIncomeCircle->DividingLine4 = p;
                break;
                
            default:
                break;
        }
        
        angle += deltaAngle;
    }
    global_memory_pool.ReleaseMemory(gl_line_data);
}

void SimpleBooks::SetCircle(SettingsCricle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba)
{
    if (innerdiameter <= 0 || thickness <= 0 || v_num == 0 || rgba == nullptr) {
        *pCircle = nullptr;
        return;
    }
    
    float* gl_data;
    global_memory_pool.GetMemory(v_num * 7 * sizeof(float), (void**)&gl_data);
    GetCircle(innerdiameter, thickness, rgba, &gl_data);
    
    SettingsCricle::CreatePolygonTouchResponder(gl_data, v_num, pCircle);
    (*pCircle)->SetRenderMode(GL_TRIANGLE_STRIP);
    global_memory_pool.ReleaseMemory(gl_data);
    (*pCircle)->radius = innerdiameter + thickness;
    (*pCircle)->insideRadius = innerdiameter;
}

void SimpleBooks::SetCircle(ButtonCricle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba)
{
    if (innerdiameter <= 0 || thickness <= 0 || v_num == 0 || rgba == nullptr) {
        *pCircle = nullptr;
        return;
    }
    
    float* gl_data;
    global_memory_pool.GetMemory(v_num * 7 * sizeof(float), (void**)&gl_data);
    GetCircle(innerdiameter, thickness, rgba, &gl_data);
    
    ButtonCricle::CreatePolygonTouchResponder(gl_data, v_num, pCircle);
    (*pCircle)->SetRenderMode(GL_TRIANGLE_STRIP);
    global_memory_pool.ReleaseMemory(gl_data);
    (*pCircle)->radius = innerdiameter + thickness;
    (*pCircle)->insideRadius = innerdiameter;
}

void SimpleBooks::SetCircle(TimeCircle** pCircle, float innerdiameter, float thickness, unsigned v_num, float* rgba)
{
    if (innerdiameter <= 0 || thickness <= 0 || v_num == 0 || rgba == nullptr) {
        *pCircle = nullptr;
        return;
    }
    
    float* gl_data;
    global_memory_pool.GetMemory(v_num * 7 * sizeof(float), (void**)&gl_data);
    GetCircle(innerdiameter, thickness, rgba, &gl_data);

    TimeCircle::CreatePolygonTouchResponder(gl_data, v_num, pCircle);
    (*pCircle)->SetRenderMode(GL_TRIANGLE_STRIP);
    global_memory_pool.ReleaseMemory(gl_data);
    (*pCircle)->radius = innerdiameter + thickness;
    (*pCircle)->insideRadius = innerdiameter;
}

void SimpleBooks::SetSiriNote(
                              float rad,
                              float r_begin,
                              float w_end,
                              unsigned v_num
                              )
{
    float* gl_data;
    global_memory_pool.GetMemory(v_num * 7 * sizeof(float), (void**)&gl_data);
    float ProgressiveWidth = w_end;// Progressive width.
    
    for(int i = 0; i < v_num / 2 + 1; ++i)
    {
        if (i == 0)
        {
            gl_data[0] = r_begin;
            gl_data[1] = 0;
            gl_data[2] = 0;
            gl_data[3] = 0;
            gl_data[4] = 1;
            gl_data[5] = 0;
            gl_data[6] = 1;
        }
        else
        {
            float ProgressiveRate = (i / (float)(v_num / 2)) * ProgressiveWidth / r_begin;// Radius progressive rate.
            float angle = (i / (float)(v_num / 2)) * rad;// Angle progressive rate.
            
            gl_data[i * 14 - 7] = r_begin * cosf(angle) * (1 - ProgressiveRate);
            gl_data[i * 14 - 6] = r_begin * sinf(angle) * (1 - ProgressiveRate);
            gl_data[i * 14 - 5] = 0;
            gl_data[i * 14 - 4] = 0;
            gl_data[i * 14 - 3] = 1;
            gl_data[i * 14 - 2] = 0;
            gl_data[i * 14 - 1] = 1;
            
            gl_data[i * 14 + 0] = r_begin * cosf(angle) * (1 + ProgressiveRate);
            gl_data[i * 14 + 1] = r_begin * sinf(angle) * (1 + ProgressiveRate);
            gl_data[i * 14 + 2] = 0;
            gl_data[i * 14 + 3] = 0;
            gl_data[i * 14 + 4] = 1;
            gl_data[i * 14 + 5] = 0;
            gl_data[i * 14 + 6] = 1;
        }
    }
    
    NotePolygon::CreatePolygonTouchResponder(gl_data, v_num, &siri_note);
    
    siri_note->SetRenderMode(GL_TRIANGLE_STRIP);
    global_memory_pool.ReleaseMemory(gl_data);
    
    siri_note->Set(rad,r_begin,w_end,v_num);
    
    global_memory_pool.GetMemory(default_precision_of_circle * 7 * sizeof(float), (void**)&gl_data);
    
    for (int i = 0; i < default_precision_of_circle; ++i) {
        float Angle = M_PI * 2 * (i + 1) / default_precision_of_circle;
        gl_data[i * 7 + 0] = r_begin * cosf(Angle);
        gl_data[i * 7 + 1] = r_begin * sinf(Angle);
        gl_data[i * 7 + 2] = 0;
        gl_data[i * 7 + 3] = 1;
        gl_data[i * 7 + 4] = 0;
        gl_data[i * 7 + 5] = 0;
        gl_data[i * 7 + 6] = 1;
    }
    
    siri_note->SetMsgOutLine(gl_data, default_precision_of_circle);
    global_memory_pool.ReleaseMemory(gl_data);
    
    //
    // Opengl之外的初始工作在Init函数完成.
    //
    siri_note->Init();
    siri_note->MsgOutline->AttachGPCObstacle();
    siri_note->MsgOutline->DetachGyroPC();
    siri_note->MsgOutline->radius = r_begin;
    siri_note->SetGraphicsOutline();
}

SimpleBooks::~SimpleBooks()
{
    SAFE_DELETE(siri_note);
    SAFE_DELETE(year);
    SAFE_DELETE(quarter);
    SAFE_DELETE(month);
    SAFE_DELETE(week);
    SAFE_DELETE(day);
    SAFE_DELETE(Settings);
    SAFE_DELETE(expensecircle);
    SAFE_DELETE(incomecircle);
    SAFE_DELETE(Money);
    SAFE_DELETE(Date);
    SAFE_DELETE(Cancel);
    SAFE_DELETE(Save);
}














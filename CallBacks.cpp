//
//  CallBacks.cpp
//  Simple Books
//
//  Created by Allen Zhao on 8/4/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "CallBacks.h"
#import "Macro.h"
#import "PolygonTouchResponder.h"
#import "SpecialPolygon.h"
#import "Math.h"
#import "SimpleBooks.h"
#import "HSDatePickerViewController.h"
#import "ViewController.h"
#import "CoreData.h"
#import "ios_sys.h"
#import "global.h"
#import "CommonFunctions.h"

void OnSave(void* args)
{
    CD.SaveMoneyToHD(pSimpleBooks->m_CurrentBillTime, pSimpleBooks->m_CurrentBillMoney, pSimpleBooks->m_CurrentBillType);
}

void OnCancel(void* args)
{
}

void OnDate(void* args)
{
    static HSDatePickerViewController *hsdpvc = [HSDatePickerViewController new];
    DropFrame();
    hsdpvc.delegate = VC;
    [VC presentViewController:hsdpvc animated:YES completion:nil];
}

void OnMoney(void*)
{
}

void OnIESwitch(void* args)
{
    IESwitchCircle* pIESwitch = (IESwitchCircle*)args;
    
    vector_2d v1 = MakeVector2d(cosf(M_PI_4), sinf(M_PI_4));
    vector_2d v2 = MakeVector2d(pIESwitch->m_CurrentSingleClickPoint.x - pIESwitch->m_Position.x,
                                pIESwitch->m_CurrentSingleClickPoint.y - pIESwitch->m_Position.y);
    
    float angle = counter_clockwise_rotation_angle(v1, v2);
    pIESwitch->OnSwitch(angle < M_PI);
}

void OnYear(void* args)
{
    TimeCircle* year = (TimeCircle*)(args);
    if (pSimpleBooks->m_AppState == _mainview) {
        pSimpleBooks->DetachAllGraphicsProcessingAndGyro();
        pSimpleBooks->RepulsionPuncture(year);
        
        year->m_LineVelocity.dx = year->m_LineVelocity.dy = 0.0f;
        
        ConstantFieldModel cfm(year->GetPosition(), pSimpleBooks->m_TheSelectedTimeDefaultCircleDockPos, 0.003, 60,year->m_AngleVelocity, year->m_AngleAcceleration, year->m_AngleVelocity, year->m_AngleAcceleration, year->GetScale(), year->GetScale(), year, year);
        year->PolygonDeforming(&cfm);
        pSimpleBooks->m_AppState = _oncheck_year;
    }
    else if(pSimpleBooks->m_AppState == _oncheck_year)
    {
        pSimpleBooks->quarter->SetPosition(year->GetPosition());
        pSimpleBooks->quarter->ToBeStatic();
        pSimpleBooks->CheckViewVisibleSwitch(pSimpleBooks->quarter);
        pSimpleBooks->m_CheckPageTime.text = GetCurrentQuarter();
        pSimpleBooks->m_AppState = _oncheck_quarter;
        pSimpleBooks->UpdateDatasLabels();
    }
}

void OnQuarter(void* args)
{
    TimeCircle* quarter = (TimeCircle*)(args);
    if (pSimpleBooks->m_AppState == _mainview) {
        pSimpleBooks->DetachAllGraphicsProcessingAndGyro();
        pSimpleBooks->RepulsionPuncture(quarter);
        
        quarter->m_LineVelocity.dx = quarter->m_LineVelocity.dy = 0.0f;
        ConstantFieldModel cfm(quarter->GetPosition(), pSimpleBooks->m_TheSelectedTimeDefaultCircleDockPos, 0.003, 60,quarter->m_AngleVelocity, quarter->m_AngleAcceleration, quarter->m_AngleVelocity, quarter->m_AngleAcceleration, quarter->GetScale(), quarter->GetScale(), quarter, quarter);
        quarter->PolygonDeforming(&cfm);
        pSimpleBooks->m_AppState = _oncheck_quarter;
    }
    else if(pSimpleBooks->m_AppState == _oncheck_quarter)
    {
        pSimpleBooks->month->SetPosition(quarter->GetPosition());
        pSimpleBooks->month->ToBeStatic();
        pSimpleBooks->CheckViewVisibleSwitch(pSimpleBooks->month);
        pSimpleBooks->m_CheckPageTime.text = GetCurrentMonth();
        pSimpleBooks->m_AppState = _oncheck_month;
        pSimpleBooks->UpdateDatasLabels();
    }
}

void OnMonth(void* args)
{
    TimeCircle* Circle = (TimeCircle*)(args);
    if (pSimpleBooks->m_AppState == _mainview) {
    pSimpleBooks->DetachAllGraphicsProcessingAndGyro();
    pSimpleBooks->RepulsionPuncture(Circle);
    
    Circle->m_LineVelocity.dx = Circle->m_LineVelocity.dy = 0.0f;
    ConstantFieldModel cfm(Circle->GetPosition(), pSimpleBooks->m_TheSelectedTimeDefaultCircleDockPos, 0.003, 60,Circle->m_AngleVelocity, Circle->m_AngleAcceleration, Circle->m_AngleVelocity, Circle->m_AngleAcceleration, Circle->GetScale(), Circle->GetScale(), Circle, Circle);
    Circle->PolygonDeforming(&cfm);
        pSimpleBooks->m_AppState = _oncheck_month;}
    else if(pSimpleBooks->m_AppState == _oncheck_month)
    {
        pSimpleBooks->week->SetPosition(Circle->GetPosition());
        pSimpleBooks->week->ToBeStatic();
        pSimpleBooks->CheckViewVisibleSwitch(pSimpleBooks->week);
        pSimpleBooks->m_CheckPageTime.text = GetCurrentWeek();
        pSimpleBooks->m_AppState = _oncheck_week;
        pSimpleBooks->UpdateDatasLabels();
    }
}

void OnWeek(void* args)
{
    TimeCircle* Circle = (TimeCircle*)(args);
    if (pSimpleBooks->m_AppState == _mainview) {
    pSimpleBooks->DetachAllGraphicsProcessingAndGyro();
    pSimpleBooks->RepulsionPuncture(Circle);
    
    Circle->m_LineVelocity.dx = Circle->m_LineVelocity.dy = 0.0f;
    ConstantFieldModel cfm(Circle->GetPosition(), pSimpleBooks->m_TheSelectedTimeDefaultCircleDockPos, 0.003, 60,Circle->m_AngleVelocity, Circle->m_AngleAcceleration, Circle->m_AngleVelocity, Circle->m_AngleAcceleration, Circle->GetScale(), Circle->GetScale(), Circle, Circle);
    Circle->PolygonDeforming(&cfm);
        pSimpleBooks->m_AppState = _oncheck_week;}
    else if(pSimpleBooks->m_AppState == _oncheck_week)
    {
        pSimpleBooks->day->SetPosition(Circle->GetPosition());
        pSimpleBooks->day->ToBeStatic();
        pSimpleBooks->CheckViewVisibleSwitch(pSimpleBooks->day);
        pSimpleBooks->m_CheckPageTime.text = GetCurrentDay();
        pSimpleBooks->m_AppState = _oncheck_day;
        pSimpleBooks->UpdateDatasLabels();
    }
}

void OnDay(void* args)
{
    TimeCircle* Circle = (TimeCircle*)(args);
    if (pSimpleBooks->m_AppState == _mainview) {
    pSimpleBooks->DetachAllGraphicsProcessingAndGyro();
    pSimpleBooks->RepulsionPuncture(Circle);
    
    Circle->m_LineVelocity.dx = Circle->m_LineVelocity.dy = 0.0f;
    ConstantFieldModel cfm(Circle->GetPosition(), pSimpleBooks->m_TheSelectedTimeDefaultCircleDockPos, 0.003, 60,Circle->m_AngleVelocity, Circle->m_AngleAcceleration, Circle->m_AngleVelocity, Circle->m_AngleAcceleration, Circle->GetScale(), Circle->GetScale(), Circle, Circle);
    Circle->PolygonDeforming(&cfm);
        pSimpleBooks->m_AppState = _oncheck_day;
    }
    
    else if(pSimpleBooks->m_AppState == _oncheck_day)
    {
        pSimpleBooks->year->SetPosition(Circle->GetPosition());
        pSimpleBooks->year->ToBeStatic();
        pSimpleBooks->CheckViewVisibleSwitch(pSimpleBooks->year);
        pSimpleBooks->m_CheckPageTime.text = GetCurrentYear();
        pSimpleBooks->m_AppState = _oncheck_year;
        pSimpleBooks->UpdateDatasLabels();
    }
}

void OnNextPeriod(void*)
{
    pSimpleBooks->UpdateCheckDatas(true);
    pSimpleBooks->UpdateDatasLabels();
}

void OnPrevoiusPeriod(void*)
{
    pSimpleBooks->UpdateCheckDatas(false);
    pSimpleBooks->UpdateDatasLabels();
}

void OnTimeCFMEnd(void* args)
{
    pSimpleBooks->TimeCfmEnd(args);
}

void OnSettings(void* args)
{
    TimeCircle* Circle = (TimeCircle*)(args);
    
    pSimpleBooks->DetachAllGraphicsProcessingAndGyro();
    pSimpleBooks->RepulsionPuncture(Circle);
    
    Circle->m_LineVelocity.dx = Circle->m_LineVelocity.dy = 0.0f;
    ConstantFieldModel cfm(Circle->GetPosition(), pSimpleBooks->m_TheSelectedTimeDefaultCircleDockPos, 0.003, 60,Circle->m_AngleVelocity, Circle->m_AngleAcceleration, Circle->m_AngleVelocity, Circle->m_AngleAcceleration, Circle->GetScale(), Circle->GetScale(), Circle, Circle);
    Circle->PolygonDeforming(&cfm);
    pSimpleBooks->m_AppState = _oncheck_settings;
}















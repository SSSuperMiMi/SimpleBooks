//
//  SpecialPolygon.h
//  Simple Books
//
//  Created by Allen Zhao on 7/14/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__SpecialPolygon__
#define __Simple_Books__SpecialPolygon__

#include <stdio.h>
#include "PolygonTouchResponder.h"

class Circle : public PolygonTouchResponder
{
public:
    Circle(float*, unsigned);
    ~Circle();
    
    float radius;
    float insideRadius;

    void AttachGPC();
    void DetachGPC();
    void AttachGPCObstacle();
    void DetachGPCObstacle();
    
    virtual void Dock();
    virtual void UnDock();
    
    vector_2d m_InitialVelocity;
    
    static bool CreatePolygonTouchResponder(float*, unsigned, Circle**);
};

class EditCircle : public Circle
{
public:
    EditCircle(float*, unsigned);
    ~EditCircle();
    
    void Init();
    
    virtual void Rendering();
    virtual void SetVisible(bool);
    
    UITextField* EditingNotes;
    static bool CreatePolygonTouchResponder(float*, unsigned, EditCircle**);
};

class TimeCircle : public Circle
{
public:
    TimeCircle(float*, unsigned);
    ~TimeCircle();
    
    void Init(unsigned);
    void InitSignCircle();
    void SignCircleRebuild();
    
    virtual void Update();
    virtual void Rendering();
    virtual void SetVisible(bool);
    
    UILabel* time_period_name;
    
    static bool CreatePolygonTouchResponder(float*, unsigned, TimeCircle**);
    
    float m_income;
    float m_expense;
    float m_budget_income;
    float m_budget_expense;
    
    float m_BudgetIncomeRatio;
    float m_BudgetExpenseRatio;
    
    float m_income_salary;
    float m_income_bonus;
    float m_income_change;
    float m_income_investment;
    float m_income_others;
    
    float m_expense_food;
    float m_expense_chothes;
    float m_expense_traffic;
    float m_expense_housing;
    float m_expense_others;
    
    void UpdateDatasByTime(NSDate*);
    
    vector_2d m_income_sign_pos_offset;
    vector_2d m_expense_sign_pos_offset;
    
    NSDate* m_CurrentRunTime;
    
    PolygonTouchResponder* m_incomeSign;
    PolygonTouchResponder* m_expenseSign;
    
    unsigned m_time_type;
};

class ButtonCricle : public Circle
{
public:
    ButtonCricle(float*, unsigned);
    ~ ButtonCricle();
    
    void Init(const char*);
    
    virtual void Rendering();
    virtual void SetVisible(bool);
    
    UILabel* Text;
    
    static bool CreatePolygonTouchResponder(float*, unsigned, ButtonCricle**);
};

class SettingsCricle : public Circle
{
public:
    SettingsCricle(float*, unsigned);
    ~SettingsCricle();
    
    void Init();
    
    virtual void Rendering();
    virtual void SetVisible(bool);
    
    UILabel* Settings;
    
    static bool CreatePolygonTouchResponder(float*, unsigned, SettingsCricle**);
};

class NotePolygon : public PolygonTouchResponder
{
public:
    NotePolygon(float*, unsigned);
    ~NotePolygon();
    
    virtual void PreparePosition();
    
    static bool CreatePolygonTouchResponder(float*, unsigned, NotePolygon**);
    
    virtual void SetPosition(point_2d);
    virtual void Rendering();
    virtual void SetVisible(bool);
    
    void SetMsgOutLine(float*, unsigned);
    void SetGraphicsOutline();
    void Set(float, float, float, unsigned);
    
    float m_rad;
    float m_r_begin;
    float m_w_end;
    
    unsigned m_v_num;
    vector_2d center_offset;
    point_2d Revolution_center;
    
    float m_Revolution_v;
    float CycleTime;
    float AngularVelocityOfRevolution;
    
    void SetMoonLikeRevolution(point_2d, float);
    void Init();
    
    static void OnNote(void*);
    static void OncfmEnd(void*);
    
    Circle* MsgOutline;
    TimeCircle* GraphicsOutline;
    
    UILabel* NewNote;
};

class ExpenseCircle : public Circle
{
public:
    static bool CreatePolygonTouchResponder(float*, unsigned, ExpenseCircle**);
    
    void Init();
    
    virtual void Update();
    virtual void SetVisible(bool);
    virtual void Rendering();
    virtual void OSMove(point_2d, point_2d);
    virtual void DoWithFingerTouchVelocity(point_2d StartPoint);
    
    PolygonTouchResponder* DividingLine0;
    UILabel* Clothes;
    PolygonTouchResponder* DividingLine1;
    UILabel* Food;
    PolygonTouchResponder* DividingLine2;
    UILabel* Housing;
    PolygonTouchResponder* DividingLine3;
    UILabel* Traffic;
    PolygonTouchResponder* DividingLine4;
    UILabel* Others;
    
    static float damped_coefficient;
    
    ExpenseCircle(float*, unsigned);
    ~ExpenseCircle();
};

class IncomeCircle : public Circle
{
public:
    static bool CreatePolygonTouchResponder(float*, unsigned, IncomeCircle**);
    
    void Init();
    
    virtual void Update();
    virtual void SetVisible(bool);
    virtual void Rendering();
    virtual void OSMove(point_2d, point_2d);
    virtual void DoWithFingerTouchVelocity(point_2d StartPoint);
    
    PolygonTouchResponder* DividingLine0;
    UILabel* Salary;
    PolygonTouchResponder* DividingLine1;
    UILabel* Bonus;
    PolygonTouchResponder* DividingLine2;
    UILabel* Change;
    PolygonTouchResponder* DividingLine3;
    UILabel* Investment;
    PolygonTouchResponder* DividingLine4;
    UILabel* Others;
    
    static float damped_coefficient;
    
    IncomeCircle(float*, unsigned);
    ~IncomeCircle();
};

class IESwitchCircle : public Circle
{
public:
    static bool CreatePolygonTouchResponder(float*, unsigned, IESwitchCircle**);
    
    void Init();
    
    virtual void SetVisible(bool);
    virtual void Rendering();
    
    void OnSwitch(bool);
    
    UILabel* Income;
    PolygonTouchResponder* DividingLine;
    UILabel* Expense;
    
    IESwitchCircle(float*, unsigned);
    ~IESwitchCircle();
};










































#endif /* defined(__Simple_Books__SpecialPolygon__) */

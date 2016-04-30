//
//  SpecialPolygon.cpp
//  Simple Books
//
//  Created by Allen Zhao on 7/14/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "SpecialPolygon.h"
#import "MemoryPool.h"
#import "Math.h"
#import "ios_sys.h"
#import "viewcontroller.h"
#import "GraphicsProcessingCenter.h"
#import "GyroProcessingCenter.h"
#import "SimpleBooks.h"
#import "CoreData.h"

void NotePolygon::OncfmEnd(void*args)
{
    NotePolygon* pSiri = (NotePolygon*)((PolygonTouchResponder*)args);
    pSiri->GraphicsOutline->SetVisible(true);
    pSiri->SetVisible(false);
    pSiri->NewNote.hidden = false;
    
    pSimpleBooks->Save->SetVisible(true);
    pSimpleBooks->Cancel->SetVisible(true);
    pSimpleBooks->Date->SetVisible(true);
    pSimpleBooks->Money->SetVisible(true);
    pSimpleBooks->expensecircle->SetVisible(true);
    pSimpleBooks->expensecircle->AttachEvent();
    pSimpleBooks->ieSwitchCircle->SetVisible(true);
    pSimpleBooks->ieSwitchCircle->AttachEvent();
}

void NotePolygon::OnNote(void* args)
{
    PolygonTouchResponder* MsgOutline = ((PolygonTouchResponder*)args);
    NotePolygon* pSiri = (NotePolygon*)((PolygonTouchResponder*)args)->m_pParent;

    TimeCircle* _year = pSimpleBooks->year;
    TimeCircle* _quarter = pSimpleBooks->quarter;
    TimeCircle* _month = pSimpleBooks->month;
    TimeCircle* _week = pSimpleBooks->week;
    TimeCircle* _day = pSimpleBooks->day;
    
    SettingsCricle* _Settings = pSimpleBooks->Settings;
    
    _year->Dock();
    _quarter->Dock();
    _month->Dock();
    _week->Dock();
    _day->Dock();
    _Settings->Dock();

    MsgOutline->Dock();
    pSiri->GraphicsOutline->Dock();
    
    ConstantFieldModel cfm_siri(pSiri->GetPosition(), MakePoint2D(pSiri->GetPosition().x - 0.2, ios_sys::gl_bound.y - pSiri->GraphicsOutline->radius / 2 - 0.2), 0.003, 60,pSiri->m_AngleVelocity, pSiri->m_AngleAcceleration, pSiri->m_AngleVelocity, pSiri->m_AngleAcceleration, pSiri->GetScale(), MakeVector2d(0.5, 0.5), pSiri, pSiri);
    pSiri->PolygonDeforming(&cfm_siri);
    
    memset(&_year->m_LineVelocity, 0, sizeof(vector_2d));
    ConstantFieldModel cfm_y(_year->GetPosition(), MakePoint2D(ios_sys::gl_bound.x - _year->radius / 2, -ios_sys::gl_bound.y + _year->radius / 2), 0.003, 60,_year->m_AngleVelocity, _year->m_AngleAcceleration, _year->m_AngleVelocity, _year->m_AngleAcceleration, _year->GetScale(), MakeVector2d(0.5, 0.5), _year, _year);
    _year->PolygonDeforming(&cfm_y);
    
    memset(&_quarter->m_LineVelocity, 0, sizeof(vector_2d));
    ConstantFieldModel cfm_q(_quarter->GetPosition(), MakePoint2D(ios_sys::gl_bound.x - _quarter->radius / 2, -ios_sys::gl_bound.y + _year->radius + _quarter->radius / 2), 0.003, 60,_quarter->m_AngleVelocity, _quarter->m_AngleAcceleration, _quarter->m_AngleVelocity, _quarter->m_AngleAcceleration, _quarter->GetScale(), MakeVector2d(0.5, 0.5), _quarter, _quarter);
    _quarter->PolygonDeforming(&cfm_q);
    
    memset(&_month->m_LineVelocity, 0, sizeof(vector_2d));
    ConstantFieldModel cfm_m(_month->GetPosition(), MakePoint2D(ios_sys::gl_bound.x - _month->radius / 2, -ios_sys::gl_bound.y + _year->radius + _quarter->radius + _month->radius / 2), 0.003, 60,_month->m_AngleVelocity, _month->m_AngleAcceleration, _month->m_AngleVelocity, _month->m_AngleAcceleration, _month->GetScale(), MakeVector2d(0.5, 0.5), _month, _month);
    _month->PolygonDeforming(&cfm_m);
    
    memset(&_week->m_LineVelocity, 0, sizeof(vector_2d));
    ConstantFieldModel cfm_w(_week->GetPosition(), MakePoint2D(ios_sys::gl_bound.x - _week->radius / 2, -ios_sys::gl_bound.y + _year->radius + _quarter->radius + _month->radius + _week->radius / 2), 0.003, 60,_week->m_AngleVelocity, _week->m_AngleAcceleration, _week->m_AngleVelocity, _week->m_AngleAcceleration, _week->GetScale(), MakeVector2d(0.5, 0.5), _week, _week);
    _week->PolygonDeforming(&cfm_w);
    
    memset(&_day->m_LineVelocity, 0, sizeof(vector_2d));
    ConstantFieldModel cfm_d(_day->GetPosition(), MakePoint2D(ios_sys::gl_bound.x - _day->radius / 2, -ios_sys::gl_bound.y + _year->radius + _quarter->radius + _month->radius + _week->radius + _day->radius / 2), 0.003, 60,_day->m_AngleVelocity, _day->m_AngleAcceleration, _day->m_AngleVelocity, _day->m_AngleAcceleration, _day->GetScale(), MakeVector2d(0.5, 0.5), _day, _day);
    _day->PolygonDeforming(&cfm_d);
    
    memset(&_Settings->m_LineVelocity, 0, sizeof(vector_2d));
    ConstantFieldModel cfm_s(_Settings->GetPosition(), MakePoint2D(ios_sys::gl_bound.x - _Settings->radius / 2, -ios_sys::gl_bound.y + _year->radius + _quarter->radius + _month->radius + _week->radius + _day->radius + _Settings->radius / 2), 0.003, 60,_Settings->m_AngleVelocity, _Settings->m_AngleAcceleration, _Settings->m_AngleVelocity, _Settings->m_AngleAcceleration, _Settings->GetScale(), MakeVector2d(0.5, 0.5), _Settings, _Settings);
    _Settings->PolygonDeforming(&cfm_s);
    
    pSimpleBooks->m_AppState = _newnote;
}

void NotePolygon::Init()
{
    SetPosition(MakePoint2D(-0.6, 0));
    MsgOutline->RegisterCallback(NotePolygon::OnNote, enum_single_finger_single_click);
    DetachEvent();
    MsgOutline->m_pParent = this;
    RegisterCallback(NotePolygon::OncfmEnd, enum_ConstantFieldModel_end);

    float h = 44, w = 180;
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MsgOutline->m_Position);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    NewNote = [UILabel new];
    NewNote.text = @"New";
    NewNote.textColor = ios_sys::TyrantGold;
    NewNote.textAlignment = NSTextAlignmentCenter;
    NewNote.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:NewNote];
}

void NotePolygon::SetMoonLikeRevolution(point_2d center, float v)
{
    memcpy(&Revolution_center, &center, sizeof(point_2d));
    m_Revolution_v = v;
    float r = sqrtf(powf(m_Position.x - Revolution_center.x, 2) + powf(m_Position.y - Revolution_center.y, 2));
    m_AngleVelocity = AngularVelocityOfRevolution = v / r;
    CycleTime = M_PI * 2 / AngularVelocityOfRevolution;  // T = 2π / w
    m_Position = PointRotation(Revolution_center, m_Position, m_angle);
}

void NotePolygon::Rendering()
{
    float h = 44, w = 180;
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(MsgOutline->GetPosition());
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    NewNote.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    NewNote.transform = CGAffineTransformScale(CGAffineTransformIdentity, m_scale_x, m_scale_y);
    GraphicsOutline->SetScale(m_scale_x, m_scale_y);
    
    PolygonTouchResponder::Rendering();
}

void NotePolygon::SetPosition(point_2d pos)
{
    if (MsgOutline != nullptr) {
        MsgOutline->SetPosition(pos);
    }
    if (GraphicsOutline != nullptr) {
        GraphicsOutline->SetPosition(pos);
    }
    
    pos.x += center_offset.dx;
    pos.y += center_offset.dy;
    
    PolygonTouchResponder::SetPosition(pos);
    SetMoonLikeRevolution(MsgOutline->GetPosition(), 0.0001);
}

void NotePolygon::SetGraphicsOutline()
{
    float color[] = {0,1,0,1};
    pSimpleBooks->SetCircle(&GraphicsOutline, MsgOutline->radius, default_circle_thickness, default_precision_of_circle_double, color);
    GraphicsOutline->DetachGyroPC();
    GraphicsOutline->DetachEvent();
    GraphicsOutline->SetPosition(MsgOutline->GetPosition());
    GraphicsOutline->SetVisible(false);
}

void NotePolygon::SetMsgOutLine(float* data, unsigned vertices_num)
{
    Circle::CreatePolygonTouchResponder(data, vertices_num, &MsgOutline);
    MsgOutline->SetVisible(false);
    MsgOutline->AttachEvent();
}

void NotePolygon::Set(float rad, float r_begin, float w_end, unsigned v_num)
{
    m_rad = rad;
    m_r_begin = r_begin;
    m_w_end = w_end;
    m_v_num = v_num;
}

NotePolygon::~NotePolygon()
{
    SAFE_DELETE(MsgOutline);
    SAFE_DELETE(GraphicsOutline);
}

void NotePolygon::PreparePosition()
{
    PolygonTouchResponder::PreparePosition();
    
    if (m_Revolution_v != 0) {
        m_Position = PointRotation(Revolution_center, m_Position, M_PI * 2 * deltaMS / CycleTime);
    }
}

void NotePolygon::SetVisible(bool b)
{
    NewNote.hidden = !b;
    PolygonTouchResponder::SetVisible(b);
}

NotePolygon::NotePolygon(float* data, unsigned vertices_num):PolygonTouchResponder(data, vertices_num),MsgOutline(nullptr), CycleTime(0), AngularVelocityOfRevolution(0), GraphicsOutline(nullptr)
{
    unsigned mm_length =  vertices_num * sizeof(point_2d);
    point_2d* v_2d;
    global_memory_pool.GetMemory(mm_length, (void**)&v_2d);
    for (int i = 0; i < vertices_num; ++i) {
        memcpy(&v_2d[i], &data[i * 7], sizeof(point_2d));
    }
    
    center_offset.dx = 0.0f;
    center_offset.dy = 0.0f;
    
    float area = 0.0f;
    float p1X  = 0.0f;
    float p1Y  = 0.0f;
    float inv3 = 1.0f/3.0f;
    
    for(int i = 0; i < m_vertices_num; ++i) {
        point_2d p2 = v_2d[i];
        point_2d p3 = (i+1)<m_vertices_num ? v_2d[i+1] : v_2d[0];
        
        float e1X = p2.x - p1X;
        float e1Y = p2.y - p1Y;
        float e2X = p3.x - p1X;
        float e2Y = p3.y - p1Y;
        float D = e1X * e2Y - e1Y * e2X;
        float triangleArea = 0.5f * D;
        
        area += triangleArea;
        
        center_offset.dx += triangleArea * inv3 * (p1X + p2.x + p3.x);
        center_offset.dy += triangleArea * inv3 * (p1Y + p2.y + p3.y);
    }
    
    if(area > FLT_EPSILON) {
        center_offset.dx *=1.0f/area;
        center_offset.dy *=1.0f/area;
    } else {
        center_offset.dx = 0xffffffff;
        center_offset.dy = 0xffffffff;
    }
    
    m_Position.x += center_offset.dx;
    m_Position.y += center_offset.dy;
    
    Revolution_center.x = Revolution_center.y = 0.0f;
    m_Revolution_v = 0.0f;
}

bool NotePolygon::CreatePolygonTouchResponder(float* data, unsigned vertices_num, NotePolygon** pPolygonTouchResponder)
{
    if(data == nullptr || vertices_num < 3){*pPolygonTouchResponder = nullptr; return false;}
    return *pPolygonTouchResponder = new NotePolygon(data, vertices_num);
}

void TimeCircle::SetVisible(bool b)
{
    time_period_name.hidden = !b;
    if(m_incomeSign != nullptr)
    {
        m_incomeSign->SetVisible(b);
    }
    if (m_expenseSign != nullptr) {
        m_expenseSign->SetVisible(b);
    }
    PolygonTouchResponder::SetVisible(b);
}

bool TimeCircle::CreatePolygonTouchResponder(float* data, unsigned vertices_num, TimeCircle** pPolygonTouchResponder)
{
    if(data == nullptr || vertices_num < 3){*pPolygonTouchResponder = nullptr; return false;}
    return *pPolygonTouchResponder = new TimeCircle(data, vertices_num);
}

void TimeCircle::Update()
{
    m_CurrentRunTime = ios_sys::GetUTCSysTimeNSDate();
    PolygonTouchResponder::Update();
}

void TimeCircle::Rendering()
{
    float h = 44, w = 180;
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(m_Position);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    time_period_name.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    time_period_name.transform = CGAffineTransformScale(CGAffineTransformIdentity, m_scale_x, m_scale_y);
    
    m_income_sign_pos_offset.dx = -radius * 0.7 * cosf(M_PI * 3 / 8) * m_scale_x;
    m_income_sign_pos_offset.dy = -radius * 0.7 * sinf(M_PI * 3 / 8) * m_scale_x;
    
    m_expense_sign_pos_offset.dx = radius * 0.7 * cosf(M_PI * 3 / 8) * m_scale_x;
    m_expense_sign_pos_offset.dy = -radius * 0.7 * sinf(M_PI * 3 / 8) * m_scale_x;
    
    if(m_incomeSign != nullptr)
    {
        m_incomeSign->SetPosition(m_Position);
        m_incomeSign->SetScale(m_scale_x, m_scale_y);
        m_incomeSign->Move(m_income_sign_pos_offset);
    }
    if (m_expenseSign != nullptr) {
        m_expenseSign->SetPosition(m_Position);
        m_expenseSign->SetScale(m_scale_x, m_scale_y);
        m_expenseSign->Move(m_expense_sign_pos_offset);
    }
    
    PolygonTouchResponder::Rendering();
}

void TimeCircle::SignCircleRebuild()
{
    // 参考颜色渐变算法. http://blog.csdn.net/phker/article/details/44401493
    float ri = 0, gi = 0, re = 0, ge = 0;
    
    if (m_BudgetIncomeRatio < 0.5 ) {
        ri = 1;
        gi = m_BudgetIncomeRatio / 0.5;
    }
    else
    {
        ri = (1 - m_BudgetIncomeRatio) / 0.5;
        gi = 1;
    }
    if (m_BudgetExpenseRatio < 0.5) {
        ge = 1;
        re = m_BudgetExpenseRatio / 0.5;
    }
    else
    {
        ge = (1 - m_BudgetExpenseRatio) / 0.5;
        re = 1;
    }
    float Income_color[] = {ri, gi, 0, 1};
    float* gl_data;
    global_memory_pool.GetMemory(default_precision_of_solid_circle * 7 * sizeof(float), (void**)(&gl_data));
    GetSolidRound(signCircleRadius, Income_color, &gl_data);
    m_incomeSign->ModelRemap(gl_data, default_precision_of_solid_circle);
    global_memory_pool.ReleaseMemory(gl_data);
    
    float Expense_color[] = {re, ge, 0, 1};
    global_memory_pool.GetMemory(default_precision_of_solid_circle * 7 * sizeof(float), (void**)(&gl_data));
    GetSolidRound(signCircleRadius, Expense_color, &gl_data);
    m_expenseSign->ModelRemap(gl_data, default_precision_of_solid_circle);
    global_memory_pool.ReleaseMemory(gl_data);
}

void TimeCircle::InitSignCircle()
{
    // 参考颜色渐变算法. http://blog.csdn.net/phker/article/details/44401493
    float ri = 0, gi = 0, re = 0, ge = 0;

    if (m_BudgetIncomeRatio < 0.5 ) {
        ri = 1;
        gi = m_BudgetIncomeRatio / 0.5;
    }
    else
    {
        ri = (1 - m_BudgetIncomeRatio) / 0.5;
        gi = 1;
    }
    if (m_BudgetExpenseRatio < 0.5) {
        ge = 1;
        re = m_BudgetExpenseRatio / 0.5;
    }
    else
    {
        ge = (1 - m_BudgetExpenseRatio) / 0.5;
        re = 1;
    }
    float Income_color[] = {ri, gi, 0, 1};
    float* gl_data;
    global_memory_pool.GetMemory(default_precision_of_solid_circle * 7 * sizeof(float), (void**)(&gl_data));
    GetSolidRound(signCircleRadius, Income_color, &gl_data);
    PolygonTouchResponder::CreatePolygonTouchResponder(gl_data, default_precision_of_solid_circle, &m_incomeSign);
    global_memory_pool.ReleaseMemory(gl_data);
    
    float Expense_color[] = {re, ge, 0, 1};
    global_memory_pool.GetMemory(default_precision_of_solid_circle * 7 * sizeof(float), (void**)(&gl_data));
    GetSolidRound(signCircleRadius, Expense_color, &gl_data);
    PolygonTouchResponder::CreatePolygonTouchResponder(gl_data, default_precision_of_solid_circle, &m_expenseSign);
    global_memory_pool.ReleaseMemory(gl_data);
}

void TimeCircle::UpdateDatasByTime(NSDate* Date)
{
    switch (m_time_type) {
        case period_type_year:
            m_income = CD.GetTargetTimeMoney(period_type_year, All_income, Date);
            m_budget_income = CD.GetBudget(_budget_year_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            
            m_expense = CD.GetTargetTimeMoney(period_type_year, All_expense, Date);
            m_budget_expense = CD.GetBudget(_budget_year_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            
            break;
            
        case period_type_quarter:
            m_income = CD.GetTargetTimeMoney(period_type_quarter, All_income, Date);
            m_budget_income = CD.GetBudget(_budget_quarter_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            m_expense = CD.GetTargetTimeMoney(period_type_quarter, All_expense, Date);
            m_budget_expense = CD.GetBudget(_budget_quarter_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            break;
            
        case period_type_month:
            m_income = CD.GetTargetTimeMoney(period_type_month, All_income, Date);
            m_budget_income = CD.GetBudget(_budget_month_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            m_expense = CD.GetTargetTimeMoney(period_type_month, All_expense, Date);
            m_budget_expense = CD.GetBudget(_budget_month_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            break;
            
        case period_type_week:
            m_income = CD.GetTargetTimeMoney(period_type_week, All_income, Date);
            m_budget_income = CD.GetBudget(_budget_week_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            m_expense = CD.GetTargetTimeMoney(period_type_week, All_expense, Date);
            m_budget_expense = CD.GetBudget(_budget_week_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            break;
            
        case period_type_day:
            m_income = CD.GetTargetTimeMoney(period_type_day, All_income, Date);
            m_budget_income = CD.GetBudget(_budget_day_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            m_expense = CD.GetTargetTimeMoney(period_type_day, All_expense, Date);
            m_budget_expense = CD.GetBudget(_budget_day_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            break;
    }
    
    m_income_bonus = CD.GetTargetTimeMoney(m_time_type, income_bonus, Date);
    m_income_change = CD.GetTargetTimeMoney(m_time_type, income_change, Date);
    m_income_investment = CD.GetTargetTimeMoney(m_time_type, income_investment, Date);
    m_income_salary = CD.GetTargetTimeMoney(m_time_type, income_salary, Date);
    m_income_others = CD.GetTargetTimeMoney(m_time_type, income_others, Date);
    m_expense_traffic = CD.GetTargetTimeMoney(m_time_type, expense_traffic, Date);
    m_expense_chothes = CD.GetTargetTimeMoney(m_time_type, expense_clothes, Date);
    m_expense_food = CD.GetTargetTimeMoney(m_time_type, expense_food, Date);
    m_expense_housing = CD.GetTargetTimeMoney(m_time_type, expense_housing, Date);
    m_expense_others = CD.GetTargetTimeMoney(m_time_type, expense_others, Date);
    
    SignCircleRebuild();
}

void TimeCircle::Init(unsigned type)
{
    m_time_type = type;
    float h = 44, w = 180;
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(m_Position);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 4;
    
    time_period_name = [UILabel new];
    time_period_name.numberOfLines = 0;
    time_period_name.lineBreakMode = NSLineBreakByWordWrapping;
    
    switch (type) {
        case period_type_year:
            time_period_name.text = @"Year";
            break;
            
        case period_type_quarter:
            time_period_name.text = @"Quarter";
            break;
            
        case period_type_month:
            time_period_name.text = @"Month";
            break;
            
        case period_type_week:
            time_period_name.text = @"Week";
            break;
            
        case period_type_day:
            time_period_name.text = @"Day";
            break;
            
        case _save:
            time_period_name.text = @"Save";
            break;
            
        case _cancel:
            time_period_name.text = @"Cancel";
            break;
            
        case _date:
            time_period_name.text = ios_sys::GetCurrentDateBlock();
            break;
            
        default:
            break;
    }

    time_period_name.textColor = ios_sys::TyrantGold;
    time_period_name.textAlignment = NSTextAlignmentCenter;
    time_period_name.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    if (type < _save) {
        time_period_name.transform = CGAffineTransformScale(CGAffineTransformIdentity, 0.7 * powf(time_circle_scale_factor, type), 0.7 * powf(time_circle_scale_factor, type));
        
        m_LineVelocity.dx = (float)GetRandNum(3, 5) / 10000;
        m_LineVelocity.dy = (float)GetRandNum(3, 5) / 10000;
        
        if (rand() % 2 == 0) {
            m_LineVelocity.dx *= -1;
        }
        if (rand() % 2 == 0) {
            m_LineVelocity.dy *= -1;
        }
        
        m_InitialVelocity.dx = m_LineVelocity.dx;
        m_InitialVelocity.dy = m_LineVelocity.dy;
    }
    
    [MainView addSubview:time_period_name];
    m_CurrentRunTime = ios_sys::GetUTCSysTimeNSDate();
    
    switch (type) {
        case period_type_year:
            m_income = CD.GetTargetTimeMoney(period_type_year, All_income);
            m_budget_income = CD.GetBudget(_budget_year_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            
            m_expense = CD.GetTargetTimeMoney(period_type_year, All_expense);
            m_budget_expense = CD.GetBudget(_budget_year_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            
            break;
            
        case period_type_quarter:
            m_income = CD.GetTargetTimeMoney(period_type_quarter, All_income);
            m_budget_income = CD.GetBudget(_budget_quarter_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            m_expense = CD.GetTargetTimeMoney(period_type_quarter, All_expense);
            m_budget_expense = CD.GetBudget(_budget_quarter_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            break;
            
        case period_type_month:
            m_income = CD.GetTargetTimeMoney(period_type_month, All_income);
            m_budget_income = CD.GetBudget(_budget_month_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            m_expense = CD.GetTargetTimeMoney(period_type_month, All_expense);
            m_budget_expense = CD.GetBudget(_budget_month_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            break;
            
        case period_type_week:
            m_income = CD.GetTargetTimeMoney(period_type_week, All_income);
            m_budget_income = CD.GetBudget(_budget_week_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            m_expense = CD.GetTargetTimeMoney(period_type_week, All_expense);
            m_budget_expense = CD.GetBudget(_budget_week_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            break;
            
        case period_type_day:
            m_income = CD.GetTargetTimeMoney(period_type_day, All_income);
            m_budget_income = CD.GetBudget(_budget_day_income);
            m_BudgetIncomeRatio = m_income / m_budget_income;
            if (m_BudgetIncomeRatio > 1) {
                m_BudgetIncomeRatio = 1;
            }
            m_expense = CD.GetTargetTimeMoney(period_type_day, All_expense);
            m_budget_expense = CD.GetBudget(_budget_day_expense);
            m_BudgetExpenseRatio = m_expense / m_budget_expense;
            if (m_BudgetExpenseRatio > 1) {
                m_BudgetExpenseRatio = 1;
            }
            break;
            
        case _save:
            time_period_name.text = @"Save";
            break;
            
        case _cancel:
            time_period_name.text = @"Cancel";
            break;
            
        case _date:
            time_period_name.text = ios_sys::GetCurrentDateBlock();
            break;
            
        default:
            break;
    }
    
    if (type < _save) {
        m_income_bonus = CD.GetTargetTimeMoney(type, income_bonus);
        m_income_change = CD.GetTargetTimeMoney(type, income_change);
        m_income_investment = CD.GetTargetTimeMoney(type, income_investment);
        m_income_salary = CD.GetTargetTimeMoney(type, income_salary);
        m_income_others = CD.GetTargetTimeMoney(type, income_others);
        m_expense_traffic = CD.GetTargetTimeMoney(type, expense_traffic);
        m_expense_chothes = CD.GetTargetTimeMoney(type, expense_clothes);
        m_expense_food = CD.GetTargetTimeMoney(type, expense_food);
        m_expense_housing = CD.GetTargetTimeMoney(type, expense_housing);
        m_expense_others = CD.GetTargetTimeMoney(type, expense_others);
        
        InitSignCircle();
    }
}

void Circle::Dock()
{
    DetachGPC();
    PolygonTouchResponder::Dock();
}

void Circle::UnDock()
{
    AttachGPC();
    PolygonTouchResponder::UnDock();
}

void Circle::AttachGPCObstacle()
{
    _gpc_.AddStaticObstacles(this);
}

void Circle::DetachGPCObstacle()
{
    _gpc_.RemoveStaticObstacles(this);
}

void Circle::AttachGPC()
{
    _gpc_.Add(this);
}

void Circle::DetachGPC()
{
    _gpc_.Remove(this);
}

Circle::Circle(float* data, unsigned vertices_num):PolygonTouchResponder(data, vertices_num), radius(0), insideRadius(0)
{
    memset(&GyroscopeSpeedBonus, 0, sizeof(vector_2d));
    memset(&m_InitialVelocity, 0, sizeof(vector_2d));
}

Circle::~Circle()
{
}

bool Circle::CreatePolygonTouchResponder(float* data, unsigned vertices_num, Circle** pPolygonTouchResponder)
{
    if(data == nullptr || vertices_num < 3){*pPolygonTouchResponder = nullptr; return false;}
    return *pPolygonTouchResponder = new Circle(data, vertices_num);
}

void EditCircle::SetVisible(bool b)
{
    EditingNotes.hidden = !b;
    PolygonTouchResponder::SetVisible(b);
}

EditCircle::EditCircle(float* data, unsigned vertices_num):Circle(data, vertices_num)
{
}

EditCircle::~EditCircle()
{
}

void EditCircle::Init()
{
    float h = radius * sqrtf(2) - default_circle_thickness;
    float w = radius * sqrtf(2) - default_circle_thickness + 0.2;
    
    h = ios_sys::gl_to_logic(h);
    w = ios_sys::gl_to_logic(w);
    
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(m_Position);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    EditingNotes = [UITextField new];
    EditingNotes.text = @"0.00$";
    EditingNotes.textColor = ios_sys::TyrantGold;
    EditingNotes.textAlignment = NSTextAlignmentCenter;
    EditingNotes.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    EditingNotes.font = [UIFont boldSystemFontOfSize:15];
    EditingNotes.keyboardAppearance = UIKeyboardAppearanceDefault;
    EditingNotes.returnKeyType = UIReturnKeyDone;
    EditingNotes.keyboardType = UIKeyboardTypeDecimalPad;
    EditingNotes.adjustsFontSizeToFitWidth=YES;
    
    [MainView addSubview:EditingNotes];
}

void EditCircle::Rendering()
{
    PolygonTouchResponder::Rendering();
}

bool EditCircle::CreatePolygonTouchResponder(float* data, unsigned vertices_num, EditCircle** pEditCircle)
{
    if(data == nullptr || vertices_num < 3){*pEditCircle = nullptr; return false;}
    return *pEditCircle = new EditCircle(data, vertices_num);
}

TimeCircle::TimeCircle(float* data, unsigned vertices_num):Circle(data, vertices_num), m_incomeSign(nullptr), m_expenseSign(nullptr), m_time_type(0)
{
    _GyroPC_.Add(this);
}

TimeCircle::~TimeCircle()
{
}

ButtonCricle::ButtonCricle(float* data, unsigned vertices_num):Circle(data, vertices_num)
{
}

ButtonCricle::~ButtonCricle()
{
}

void ButtonCricle::Init(const char* text)
{
    float h = 44, w = 180;
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(m_Position);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Text = [UILabel new];
    Text.text = [NSString stringWithUTF8String:text];
    Text.textColor = ios_sys::TyrantGold;
    Text.textAlignment = NSTextAlignmentCenter;
    Text.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    
    [MainView addSubview:Text];
}

void ButtonCricle::Rendering()
{
    float h = 44 * m_scale_x, w = 180 * m_scale_y;
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(m_Position);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Text.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Text.transform = CGAffineTransformIdentity;
    Text.transform = CGAffineTransformScale(CGAffineTransformRotate(Text.transform, -m_angle),  m_scale_x, m_scale_y);
    
    PolygonTouchResponder::Rendering();
}

void ButtonCricle::SetVisible(bool b)
{
    Text.hidden = !b;
    PolygonTouchResponder::SetVisible(b);
}

bool ButtonCricle::CreatePolygonTouchResponder(float* data, unsigned vertices_num, ButtonCricle** pPolygonTouchResponder)
{
    if(data == nullptr || vertices_num < 3){*pPolygonTouchResponder = nullptr; return false;}
    return *pPolygonTouchResponder = new ButtonCricle(data, vertices_num);
}

bool SettingsCricle::CreatePolygonTouchResponder(float* data, unsigned vertices_num, SettingsCricle** pPolygonTouchResponder)
{
    if(data == nullptr || vertices_num < 3){*pPolygonTouchResponder = nullptr; return false;}
    return *pPolygonTouchResponder = new SettingsCricle(data, vertices_num);
}

void SettingsCricle::Init()
{
    float h = 44, w = 180;
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(m_Position);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Settings = [UILabel new];
    Settings.text = @"Settings";
    Settings.textColor = ios_sys::TyrantGold;
    Settings.textAlignment = NSTextAlignmentCenter;
    Settings.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Settings.transform = CGAffineTransformScale(CGAffineTransformIdentity, 0.7 * powf(time_circle_scale_factor, 5), 0.7 * powf(time_circle_scale_factor, 5));
    
    m_LineVelocity.dx = (float)GetRandNum(3, 5) / 10000;
    m_LineVelocity.dy = (float)GetRandNum(3, 5) / 10000;
    
    if (rand() % 2 == 0) {
        m_LineVelocity.dx *= -1;
    }
    if (rand() % 2 == 0) {
        m_LineVelocity.dy *= -1;
    }
    m_InitialVelocity.dx = m_LineVelocity.dx;
    m_InitialVelocity.dy = m_LineVelocity.dy;
    
    [MainView addSubview:Settings];
}

void SettingsCricle::SetVisible(bool b)
{
    Settings.hidden = !b;
    PolygonTouchResponder::SetVisible(b);
}

SettingsCricle::SettingsCricle(float* data, unsigned vertices_num):Circle(data, vertices_num)
{
}

SettingsCricle::~SettingsCricle()
{
}

void SettingsCricle::Rendering()
{
    float h = 44, w = 180;
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(m_Position);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Settings.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Settings.transform = CGAffineTransformScale(CGAffineTransformIdentity, m_scale_x, m_scale_y);
    
    PolygonTouchResponder::Rendering();
}

void ExpenseCircle::Rendering()
{
    if (!m_isVisible) {
        return;
    }
    float RevolutionAngle = m_angle;
    float angle = 0;
    float deltaangle = 0.4 * M_PI;
    
    vector_2d pos_offset;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine0->SetScale(m_scale_x, m_scale_y);
    DividingLine0->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine0->m_angle = RevolutionAngle;
    DividingLine0->SetPosition(PointRotation(m_Position, DividingLine0->GetPosition(), m_angle));
    
    float XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    float YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    
    DividingLine0->m_Position.x += XOffset;
    DividingLine0->m_Position.y += YOffset;
    
    angle += deltaangle;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine1->SetScale(m_scale_x, m_scale_y);
    DividingLine1->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine1->m_angle = RevolutionAngle;
    DividingLine1->SetPosition(PointRotation(m_Position, DividingLine1->GetPosition(), m_angle));
    
    XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    DividingLine1->m_Position.x += XOffset;
    DividingLine1->m_Position.y += YOffset;
    
    angle += deltaangle;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine2->SetScale(m_scale_x, m_scale_y);
    DividingLine2->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine2->m_angle = RevolutionAngle;
    DividingLine2->SetPosition(PointRotation(m_Position, DividingLine2->GetPosition(), m_angle));
   
    XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    DividingLine2->m_Position.x += XOffset;
    DividingLine2->m_Position.y += YOffset;
    
    angle += deltaangle;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine3->SetScale(m_scale_x, m_scale_y);
    DividingLine3->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine3->m_angle = RevolutionAngle;
    DividingLine3->SetPosition(PointRotation(m_Position, DividingLine3->GetPosition(), m_angle));
    
    XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    DividingLine3->m_Position.x += XOffset;
    DividingLine3->m_Position.y += YOffset;
    
    angle += deltaangle;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine4->SetScale(m_scale_x, m_scale_y);
    DividingLine4->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine4->m_angle = RevolutionAngle;
    DividingLine4->SetPosition(PointRotation(m_Position, DividingLine4->GetPosition(), m_angle));
    
    XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    DividingLine4->m_Position.x += XOffset;
    DividingLine4->m_Position.y += YOffset;

    float base_angle = 0.2 * M_PI;
    float delta_angle = 0.4 * M_PI;
    float h = 44, w = 180;
    
    point_2d pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;

    XOffset = (m_scale_x - 1) * insideRadius / 2 / m_scale_x;
    float lx = ios_sys::gl_to_logic(XOffset);
    
    Clothes.transform = CGAffineTransformIdentity;
    Clothes.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Clothes.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Clothes.transform = CGAffineTransformTranslate(Clothes.transform, lx, 0);
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Clothes.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = expense_clothes;
    }
    else
    {
        Clothes.textColor = ios_sys::TyrantGold;
    }
    
    base_angle += delta_angle;
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Food.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = expense_food;
    }
    else
    {
        Food.textColor = ios_sys::TyrantGold;
    }
    pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Food.transform = CGAffineTransformIdentity;
    Food.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Food.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Food.transform = CGAffineTransformTranslate(Food.transform, lx, 0);
    base_angle += delta_angle;
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Housing.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = expense_housing;
    }
    else
    {
        Housing.textColor = ios_sys::TyrantGold;
    }
    pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    Housing.transform = CGAffineTransformIdentity;
    Housing.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Housing.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Housing.transform = CGAffineTransformTranslate(Housing.transform, lx, 0);
    base_angle += delta_angle;
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Traffic.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = expense_traffic;
    }
    else
    {
        Traffic.textColor = ios_sys::TyrantGold;
    }
    pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    Traffic.transform = CGAffineTransformIdentity;
    Traffic.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Traffic.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Traffic.transform = CGAffineTransformTranslate(Traffic.transform, lx, 0);
    base_angle += delta_angle;
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Others.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = expense_others;
    }
    else
    {
        Others.textColor = ios_sys::TyrantGold;
    }
    pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    Others.transform = CGAffineTransformIdentity;
    Others.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Others.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Others.transform = CGAffineTransformTranslate(Others.transform, lx, 0);
    
    PolygonTouchResponder::Rendering();
}

void ExpenseCircle::OSMove(point_2d p1, point_2d p2)
{
    m_angle += counter_clockwise_rotation_angle(MakeVector2d(p1.x - m_Position.x, p1.y - m_Position.y), MakeVector2d(p2.x - m_Position.x, p2.y - m_Position.y));
}

void ExpenseCircle::DoWithFingerTouchVelocity(point_2d p1)
{
    point_2d p2 = MakePoint2D(p1.x + m_finger_touch_velocity.dx, p1.y + m_finger_touch_velocity.dy);
    
    m_AngleVelocity = counter_clockwise_rotation_angle(MakeVector2d(p1.x - m_Position.x, p1.y - m_Position.y), MakeVector2d(p2.x - m_Position.x, p2.y - m_Position.y));
    if (m_AngleVelocity > M_PI) {
        m_AngleVelocity = m_AngleVelocity - 2 * M_PI;
    }
}

void ExpenseCircle::Update()
{
    m_AngleAcceleration = damped_coefficient * m_AngleVelocity;
    PolygonTouchResponder::Update();
}

void ExpenseCircle::SetVisible(bool b)
{
    DividingLine0->SetVisible(b);
    Clothes.hidden = !b;
    DividingLine1->SetVisible(b);
    Food.hidden = !b;
    DividingLine2->SetVisible(b);
    Housing.hidden = !b;
    DividingLine3->SetVisible(b);
    Traffic.hidden = !b;
    DividingLine4->SetVisible(b);
    Others.hidden = !b;
    
    PolygonTouchResponder::SetVisible(b);
}

bool ExpenseCircle::CreatePolygonTouchResponder(float* data, unsigned vertices_num, ExpenseCircle** pExpenseCircle)
{
    if(data == nullptr || vertices_num < 3){*pExpenseCircle = nullptr; return false;}
    return *pExpenseCircle = new ExpenseCircle(data, vertices_num);
}

ExpenseCircle::ExpenseCircle(float* data, unsigned vertices_num):Circle(data, vertices_num),
DividingLine0(nullptr),
DividingLine1(nullptr),
DividingLine2(nullptr),
DividingLine3(nullptr),
DividingLine4(nullptr)
{
}

ExpenseCircle::~ExpenseCircle()
{
    SAFE_DELETE(DividingLine0);
    SAFE_DELETE(DividingLine1);
    SAFE_DELETE(DividingLine2);
    SAFE_DELETE(DividingLine3);
    SAFE_DELETE(DividingLine4);
}

void ExpenseCircle::Init()
{
    float base_angle = 0.2 * M_PI;
    float delta_angle = 0.4 * M_PI;
    
    float h = 44, w = 180;
    
    point_2d pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Clothes = [UILabel new];
    Clothes.numberOfLines = 0;
    Clothes.lineBreakMode = NSLineBreakByWordWrapping;
    Clothes.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Clothes.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Clothes.textColor = ios_sys::TyrantGold;
    Clothes.textAlignment = NSTextAlignmentCenter;
    Clothes.text = @"Clothes";

    [MainView addSubview:Clothes];
    
    base_angle += delta_angle;
    pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Food = [UILabel new];
    Food.numberOfLines = 0;
    Food.lineBreakMode = NSLineBreakByWordWrapping;
    Food.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Food.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Food.textColor = ios_sys::TyrantGold;
    Food.textAlignment = NSTextAlignmentCenter;
    Food.text = @"Food";
    
    [MainView addSubview:Food];
    
    base_angle += delta_angle;
    pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Housing = [UILabel new];
    Housing.numberOfLines = 0;
    Housing.lineBreakMode = NSLineBreakByWordWrapping;
    Housing.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Housing.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Housing.textColor = ios_sys::TyrantGold;
    Housing.textAlignment = NSTextAlignmentCenter;
    Housing.text = @"Housing";
    
    [MainView addSubview:Housing];
    
    base_angle += delta_angle;
    pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Traffic = [UILabel new];
    Traffic.numberOfLines = 0;
    Traffic.lineBreakMode = NSLineBreakByWordWrapping;
    Traffic.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Traffic.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Traffic.textColor = ios_sys::TyrantGold;
    Traffic.textAlignment = NSTextAlignmentCenter;
    Traffic.text = @"Traffic";
    
    [MainView addSubview:Traffic];
    
    base_angle += delta_angle;
    pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Others = [UILabel new];
    Others.numberOfLines = 0;
    Others.lineBreakMode = NSLineBreakByWordWrapping;
    Others.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Others.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Others.textColor = ios_sys::TyrantGold;
    Others.textAlignment = NSTextAlignmentCenter;
    Others.text = @"Others";

    [MainView addSubview:Others];
}

void IncomeCircle::Update()
{
    m_AngleAcceleration = damped_coefficient * m_AngleVelocity;
    PolygonTouchResponder::Update();
}

void IncomeCircle::SetVisible(bool b)
{
    DividingLine0->SetVisible(b);
    Salary.hidden = !b;
    DividingLine1->SetVisible(b);
    Bonus.hidden = !b;
    DividingLine2->SetVisible(b);
    Change.hidden = !b;
    DividingLine3->SetVisible(b);
    Investment.hidden = !b;
    DividingLine4->SetVisible(b);
    Others.hidden = !b;
    
    PolygonTouchResponder::SetVisible(b);
}

bool IncomeCircle::CreatePolygonTouchResponder(float* data, unsigned vertices_num, IncomeCircle** pExpenseCircle)
{
    if(data == nullptr || vertices_num < 3){*pExpenseCircle = nullptr; return false;}
    return *pExpenseCircle = new IncomeCircle(data, vertices_num);
}

IncomeCircle::IncomeCircle(float* data, unsigned vertices_num):Circle(data, vertices_num),
DividingLine0(nullptr),
DividingLine1(nullptr),
DividingLine2(nullptr),
DividingLine3(nullptr),
DividingLine4(nullptr)
{
}

IncomeCircle::~IncomeCircle()
{
    SAFE_DELETE(DividingLine0);
    SAFE_DELETE(DividingLine1);
    SAFE_DELETE(DividingLine2);
    SAFE_DELETE(DividingLine3);
    SAFE_DELETE(DividingLine4);
}

void IncomeCircle::DoWithFingerTouchVelocity(point_2d p1)
{
    point_2d p2 = MakePoint2D(p1.x + m_finger_touch_velocity.dx, p1.y + m_finger_touch_velocity.dy);
    
    m_AngleVelocity = counter_clockwise_rotation_angle(MakeVector2d(p1.x - m_Position.x, p1.y - m_Position.y), MakeVector2d(p2.x - m_Position.x, p2.y - m_Position.y));
    if (m_AngleVelocity > M_PI) {
        m_AngleVelocity = m_AngleVelocity - 2 * M_PI;
    }
}

void IncomeCircle::OSMove(point_2d p1, point_2d p2)
{
    m_angle += counter_clockwise_rotation_angle(MakeVector2d(p1.x - m_Position.x, p1.y - m_Position.y), MakeVector2d(p2.x - m_Position.x, p2.y - m_Position.y));
}

void IncomeCircle::Rendering()
{
    if (!m_isVisible) {
        return;
    }
    float RevolutionAngle = m_angle;
    float angle = 0;
    float deltaangle = 0.4 * M_PI;
    
    vector_2d pos_offset;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine0->SetScale(m_scale_x, m_scale_y);
    DividingLine0->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine0->m_angle = RevolutionAngle;
    DividingLine0->SetPosition(PointRotation(m_Position, DividingLine0->GetPosition(), m_angle));
    
    float XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    float YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    
    DividingLine0->m_Position.x += XOffset;
    DividingLine0->m_Position.y += YOffset;
    
    angle += deltaangle;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine1->SetScale(m_scale_x, m_scale_y);
    DividingLine1->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine1->m_angle = RevolutionAngle;
    DividingLine1->SetPosition(PointRotation(m_Position, DividingLine1->GetPosition(), m_angle));
    
    XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    
    DividingLine1->m_Position.x += XOffset;
    DividingLine1->m_Position.y += YOffset;
    
    angle += deltaangle;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine2->SetScale(m_scale_x, m_scale_y);
    DividingLine2->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine2->m_angle = RevolutionAngle;
    DividingLine2->SetPosition(PointRotation(m_Position, DividingLine2->GetPosition(), m_angle));
    
    XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    DividingLine2->m_Position.x += XOffset;
    DividingLine2->m_Position.y += YOffset;
    
    angle += deltaangle;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine3->SetScale(m_scale_x, m_scale_y);
    DividingLine3->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine3->m_angle = RevolutionAngle;
    DividingLine3->SetPosition(PointRotation(m_Position, DividingLine3->GetPosition(), m_angle));
    
    XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    DividingLine3->m_Position.x += XOffset;
    DividingLine3->m_Position.y += YOffset;
    
    angle += deltaangle;
    pos_offset.dx = insideRadius * cosf(angle) / 2;
    pos_offset.dy = insideRadius * sinf(angle) / 2;
    
    DividingLine4->SetScale(m_scale_x, m_scale_y);
    DividingLine4->SetPosition(MakePoint2D(m_Position.x + pos_offset.dx, m_Position.y + pos_offset.dy));
    DividingLine4->m_angle = RevolutionAngle;
    DividingLine4->SetPosition(PointRotation(m_Position, DividingLine4->GetPosition(), m_angle));
    
    XOffset = (m_scale_x - 1) * insideRadius * cosf(RevolutionAngle + angle) / 2;
    YOffset = (m_scale_y - 1) * insideRadius * sinf(RevolutionAngle + angle) / 2;
    DividingLine4->m_Position.x += XOffset;
    DividingLine4->m_Position.y += YOffset;
    
    float base_angle = 0.2 * M_PI;
    float delta_angle = 0.4 * M_PI;
    float h = 44, w = 180;
    
    point_2d pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    XOffset = (m_scale_x - 1) * insideRadius / 2 / m_scale_x;
    float lx = ios_sys::gl_to_logic(XOffset);
    
    Salary.transform = CGAffineTransformIdentity;
    Salary.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Salary.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Salary.transform = CGAffineTransformTranslate(Salary.transform, lx, 0);
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Salary.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = income_salary;
    }
    else
    {
        Salary.textColor = ios_sys::TyrantGold;
    }
    
    base_angle += delta_angle;
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Bonus.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = income_bonus;
    }
    else
    {
        Bonus.textColor = ios_sys::TyrantGold;
    }
    pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Bonus.transform = CGAffineTransformIdentity;
    Bonus.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Bonus.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Bonus.transform = CGAffineTransformTranslate(Bonus.transform, lx, 0);
    
    base_angle += delta_angle;
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Change.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = income_change;
    }
    else
    {
        Change.textColor = ios_sys::TyrantGold;
    }
    pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    Change.transform = CGAffineTransformIdentity;
    Change.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Change.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Change.transform = CGAffineTransformTranslate(Change.transform, lx, 0);
    
    base_angle += delta_angle;
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Investment.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = income_investment;
    }
    else
    {
        Investment.textColor = ios_sys::TyrantGold;
    }
    pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    Investment.transform = CGAffineTransformIdentity;
    Investment.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Investment.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Investment.transform = CGAffineTransformTranslate(Investment.transform, lx, 0);
    
    base_angle += delta_angle;
    if (2 * M_PI - GetBaseAngle(-base_angle-RevolutionAngle) < deltaangle) {
        Others.textColor = ios_sys::red;
        pSimpleBooks->m_CurrentBillType = income_others;
    }
    else
    {
        Others.textColor = ios_sys::TyrantGold;
    }
    pos = MakePoint2D(cosf(base_angle + RevolutionAngle) * insideRadius / 2, sinf(base_angle + RevolutionAngle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    Others.transform = CGAffineTransformIdentity;
    Others.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Others.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle - RevolutionAngle),  m_scale_x, m_scale_y);
    Others.transform = CGAffineTransformTranslate(Others.transform, lx, 0);
    
    PolygonTouchResponder::Rendering();
}

float ExpenseCircle::damped_coefficient = -0.002;
float IncomeCircle::damped_coefficient = -0.002;
void IncomeCircle::Init()
{
    float base_angle = 0.2 * M_PI;
    float delta_angle = 0.4 * M_PI;
    
    float h = 44, w = 180;
    
    point_2d pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Salary = [UILabel new];
    Salary.numberOfLines = 0;
    Salary.lineBreakMode = NSLineBreakByWordWrapping;
    Salary.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Salary.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Salary.textColor = ios_sys::TyrantGold;
    Salary.textAlignment = NSTextAlignmentCenter;
    Salary.text = @"Salary";
    
    [MainView addSubview:Salary];
    
    base_angle += delta_angle;
    pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Bonus = [UILabel new];
    Bonus.numberOfLines = 0;
    Bonus.lineBreakMode = NSLineBreakByWordWrapping;
    Bonus.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Bonus.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Bonus.textColor = ios_sys::TyrantGold;
    Bonus.textAlignment = NSTextAlignmentCenter;
    Bonus.text = @"Bonus";
    
    [MainView addSubview:Bonus];
    
    base_angle += delta_angle;
    pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Change = [UILabel new];
    Change.numberOfLines = 0;
    Change.lineBreakMode = NSLineBreakByWordWrapping;
    Change.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Change.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Change.textColor = ios_sys::TyrantGold;
    Change.textAlignment = NSTextAlignmentCenter;
    Change.text = @"Change";
    
    [MainView addSubview:Change];
    
    base_angle += delta_angle;
    pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Investment = [UILabel new];
    Investment.numberOfLines = 0;
    Investment.lineBreakMode = NSLineBreakByWordWrapping;
    Investment.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Investment.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Investment.textColor = ios_sys::TyrantGold;
    Investment.textAlignment = NSTextAlignmentCenter;
    Investment.text = @"Investment";
    
    [MainView addSubview:Investment];
    
    base_angle += delta_angle;
    pos = MakePoint2D(cosf(base_angle) * insideRadius / 2, sinf(base_angle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Others = [UILabel new];
    Others.numberOfLines = 0;
    Others.lineBreakMode = NSLineBreakByWordWrapping;
    Others.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Others.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -base_angle),  m_scale_x, m_scale_y);
    Others.textColor = ios_sys::TyrantGold;
    Others.textAlignment = NSTextAlignmentCenter;
    Others.text = @"Others";
    
    [MainView addSubview:Others];
}

void IESwitchCircle::Rendering()
{
    DividingLine->SetScale(m_scale_x, m_scale_y);
    DividingLine->SetPosition(GetPosition());
    DividingLine->m_angle = m_angle;
    
    float RevolutionAngle = m_angle;
    float h = 44, w = 180;
    
    point_2d pos = MakePoint2D(cosf(M_PI_4 * 3 + RevolutionAngle) * insideRadius / 2, sinf(M_PI_4 * 3 + RevolutionAngle) * insideRadius / 2);
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Income.transform = CGAffineTransformIdentity;
    Income.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Income.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -M_PI_4 - RevolutionAngle),  m_scale_x, m_scale_y);
    
    pos = MakePoint2D(cosf(-M_PI_4 + RevolutionAngle) * insideRadius / 2, sinf(-M_PI_4 + RevolutionAngle) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;

    Expense.transform = CGAffineTransformIdentity;
    Expense.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Expense.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -M_PI_4 - RevolutionAngle),  m_scale_x, m_scale_y);
    
    PolygonTouchResponder::Rendering();
}

void IESwitchCircle::SetVisible(bool b)
{
    DividingLine->SetVisible(b);
    Income.hidden = !b;
    Expense.hidden = !b;

    PolygonTouchResponder::SetVisible(b);
}

bool IESwitchCircle::CreatePolygonTouchResponder(float* data, unsigned vertices_num, IESwitchCircle** pIESwitchCircle)
{
    if(data == nullptr || vertices_num < 3){*pIESwitchCircle = nullptr; return false;}
    return *pIESwitchCircle = new IESwitchCircle(data, vertices_num);
}

void IESwitchCircle::Init()
{
    float h = 44, w = 180;
    
    point_2d pos = MakePoint2D(cosf(M_PI_4 * 3) * insideRadius / 2, sinf(M_PI_4) * insideRadius / 2);
    point_2d logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Income = [UILabel new];
    Income.numberOfLines = 0;
    Income.lineBreakMode = NSLineBreakByWordWrapping;
    Income.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Income.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -M_PI_4),  m_scale_x, m_scale_y);
    Income.textColor = ios_sys::TyrantGold;
    Income.textAlignment = NSTextAlignmentCenter;
    Income.text = @"Income";
    
    [MainView addSubview:Income];
    
    pos = MakePoint2D(cosf(M_PI_4) * insideRadius / 2, sinf(-M_PI_4) * insideRadius / 2);
    logic_pos = ios_sys::gl_coordinate_to_logic_coordinate(pos);
    
    logic_pos.x -= w / 2;
    logic_pos.y -= h / 2;
    
    Expense = [UILabel new];
    Expense.numberOfLines = 0;
    Expense.lineBreakMode = NSLineBreakByWordWrapping;
    Expense.frame = CGRectMake(logic_pos.x, logic_pos.y, w, h);
    Expense.transform = CGAffineTransformScale(CGAffineTransformRotate(CGAffineTransformTranslate(CGAffineTransformIdentity, ios_sys::gl_to_logic(m_Position.x), -ios_sys::gl_to_logic(m_Position.y)), -M_PI_4),  m_scale_x, m_scale_y);
    Expense.textColor = ios_sys::red;
    Expense.textAlignment = NSTextAlignmentCenter;
    Expense.text = @"Expense";
    
    [MainView addSubview:Expense];
}

void IESwitchCircle::OnSwitch(bool ISIncome)
{
    if (ISIncome == true) {
        Income.textColor = ios_sys::red;
        Expense.textColor = ios_sys::TyrantGold;
        pSimpleBooks->incomecircle->SetVisible(true);
        pSimpleBooks->incomecircle->AttachEvent();
        pSimpleBooks->expensecircle->SetVisible(false);
        pSimpleBooks->expensecircle->DetachEvent();
    }
    else
    {
        Income.textColor = ios_sys::TyrantGold;
        Expense.textColor = ios_sys::red;
        pSimpleBooks->incomecircle->SetVisible(false);
        pSimpleBooks->expensecircle->SetVisible(true);
        pSimpleBooks->incomecircle->DetachEvent();
        pSimpleBooks->expensecircle->AttachEvent();
    }
}

IESwitchCircle::IESwitchCircle(float* data, unsigned vertices_num):Circle(data, vertices_num),
Income(nullptr), Expense(nullptr), DividingLine(nullptr)
{
}

IESwitchCircle::~IESwitchCircle()
{
}




















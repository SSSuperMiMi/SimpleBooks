//
//  Enum.h
//  Simple Books
//
//  Created by Allen Zhao on 4/28/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef Simple_Books_Enum_h
#define Simple_Books_Enum_h

enum
{
    enum_no_touch_event = 0,
    
    enum_single_finger_single_click = 0x10,
    enum_single_finger_double_click,
    enum_two_finger_single_click,
    enum_two_finger_double_click,
    enum_long_press_began,
    enum_long_press_end,
    enum_pinch,
    enum_rotate,
    enum_pan,
    enum_double_pan,
    enum_ConstantFieldModel_end,
    enum_os_touch_begin,
    enum_os_touch_moved,
    enum_os_touch_ended
};

enum
{
    period_type_year = 0,
    period_type_quarter,
    period_type_month,
    period_type_week,
    period_type_day,
    
    _save,
    _cancel,
    _date
};

enum
{
    _touchesBegan = 0,
    _touchesMoved,
    _touchesEnded,
    _touchesCancelled
};

enum
{
    expense_clothes = 0,
    expense_food,
    expense_housing,
    expense_traffic,
    expense_others,
    
    income_salary,
    income_bonus,
    income_change,
    income_investment,
    income_others,
    
    All_income,
    All_expense
};

enum
{
    _budget_day_income,
    _budget_day_expense,
    _budget_week_income,
    _budget_week_expense,
    _budget_month_income,
    _budget_month_expense,
    _budget_quarter_income,
    _budget_quarter_expense,
    _budget_year_income,
    _budget_year_expense
};

enum
{
    _mainview,
    _newnote,
    _oncheck_year,
    _oncheck_quarter,
    _oncheck_month,
    _oncheck_week,
    _oncheck_day,
    _oncheck_settings
};

#endif











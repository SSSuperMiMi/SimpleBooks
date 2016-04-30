//
//  Macro.h
//  Simple Books
//
//  Created by Allen Zhao on 4/28/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef Simple_Books_Macro_h
#define Simple_Books_Macro_h

#define breakpoint_test_macro {int iiiiii = 0; ++iiiiii;}

#define SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define BREAK_IF(cond)           if(cond) break

#define os_touch_event
#define invalid_value            0xffffffff

#define vao_vertex_components_num    3
#define vao_color_components_num     4
#define vao_attribute_gap            (7 * sizeof(float))
#define vao_vertex_offset            0
#define vao_color_offset             (GLvoid*)(3 * sizeof(float))


#define shader_log_success                                  0x0

#define shader_log_path_error                               0x1
#define shader_log_failed_to_compile_vertex_shader          0x2
#define shader_log_failed_to_compile_fragment_shader        0x3
#define shader_log_failed_to_link_shader                    0x4

#define default_precision_of_circle                         0x80
#define default_precision_of_solid_circle                   (0x80 + 1)
#define default_precision_of_circle_double                  0x100

#define time_circle_scale_factor                            1.1
#define circle_deform_factor                                0.1
#define default_circle_thickness                            0.009

#define default_budget_day_income                           200
#define default_budget_day_expense                          90

#define default_budget_week_income                           1000
#define default_budget_week_expense                          630

#define default_budget_month_income                           4000
#define default_budget_month_expense                          3000

#define default_budget_quarter_income                           12000
#define default_budget_quarter_expense                          9000

#define default_budget_year_income                           50000
#define default_budget_year_expense                          36000

typedef void(*pfn_callback)(void*);

#define signCircleRadius                                    0.03
#define GravitationalConstant                               0.00008

#define invalid_value                                       0xffffffff

#endif

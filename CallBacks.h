//
//  CallBacks.h
//  Simple Books
//
//  Created by Allen Zhao on 8/4/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__CallBacks__
#define __Simple_Books__CallBacks__

#include <stdio.h>

void OnSave(void*);
void OnCancel(void*);
void OnDate(void*);
void OnMoney(void*);
void OnIESwitch(void*);

void OnYear(void*);
void OnQuarter(void*);
void OnMonth(void*);
void OnWeek(void*);
void OnDay(void*);
void OnSettings(void*);
void OnTimeCFMEnd(void*);
void OnNextPeriod(void*);
void OnPrevoiusPeriod(void*);

#endif /* defined(__Simple_Books__CallBacks__) */

//
//  TouchMsgCenter.h
//  Simple Books
//
//  Created by Allen Zhao on 5/19/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "CTimer.h"
#include <sys/select.h>

//////////////////////////public methods//////////////////////////
CTimer::CTimer():
m_second(0), m_microsecond(0)
{
}

CTimer::CTimer(long second, long microsecond, pfn_callback callback, bool repeat, void* args) :
m_second(second), m_microsecond(microsecond), m_pfn_callback(callback), m_args(args), m_repeat(repeat),
thread_timer(NULL)
{
}

CTimer::~CTimer()
{
    StopTimer();
}

void CTimer::SetTimer(long second, long microsecond)
{
    m_second = second;
    m_microsecond = microsecond;
}

void CTimer::StartTimer()
{
    pthread_create(&thread_timer, NULL, OnTimer_stub, this);
}

void CTimer::StopTimer()
{
    if(thread_timer == NULL)return;
    pthread_cancel(thread_timer);
    pthread_join(thread_timer, NULL);
    thread_timer = NULL;
    delete this;
}

//////////////////////////private methods//////////////////////////
void CTimer::thread_proc()
{
    while (true)
    {
        pthread_testcancel();
        struct timeval tempval;
        tempval.tv_sec = m_second;
        tempval.tv_usec = m_microsecond;
        select(0, NULL, NULL, NULL, &tempval);
        OnTimer();
        if(m_repeat == false){
            thread_timer = NULL;
            break;
        }
    }
    delete this;
}

void CTimer::OnTimer()
{
    if(m_pfn_callback)
    {
        m_pfn_callback(m_args);
    }
}







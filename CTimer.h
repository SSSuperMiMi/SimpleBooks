//
//  TouchMsgCenter.h
//  Simple Books
//
//  Created by Allen Zhao on 5/19/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//
#ifndef CTIMER_H_
#define CTIMER_H_

#include <pthread.h>
#include "macro.h"

class CTimer
{
private:
    pthread_t thread_timer;
    long m_second, m_microsecond;
    static void *OnTimer_stub(void *p)
    {
        (static_cast<CTimer*>(p))->thread_proc();
        return NULL;
    }
    void thread_proc();
    void OnTimer();
    
public:
    CTimer();
    CTimer(long second, long microsecond, pfn_callback callback, bool repeat, void* args);
    virtual ~CTimer();
    void SetTimer(long second,long microsecond);
    void StartTimer();
    void StopTimer();
    
    pfn_callback m_pfn_callback;
    void* m_args;
    bool m_repeat;
};
#endif /* CTIMER_H_ */

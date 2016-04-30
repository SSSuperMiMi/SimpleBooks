//
//  TouchMsgCenter.h
//  Simple Books
//
//  Created by Allen Zhao on 5/16/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//
#ifndef DataStructure_h
#define DataStructure_h

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

class Array
{
public:
    const int max_length = 0xFFFFFFF;
    
    int m_current_memory_alloc_length;
    int m_current_length;
    
    unsigned m_unit_length;
    
    void** m_pData;

    Array();
    Array(const Array&);
    Array(const Array*);
    
    ~Array();

    void*  operator[](unsigned j)const;
    
    bool DeleteObject(void*);
    bool DeleteObjectAtIndex(unsigned);
    bool AppendData(void*);
    bool Copy(Array*&);
    bool Copy(Array**);
    bool Find(void*);
    
    void* GetObjectAtIndex(unsigned);
    unsigned GetLength();
    unsigned GetIndex(void*);
    bool IsEmpty();
    
    void Clear();
    void Log();
    
protected:
    bool InsertAtEnd(void*);
    bool InsertAtHead(void*);
};

class Stack : public Array
{
public:
    Stack();
    Stack(const Stack&);
    Stack(const Stack*);
    ~Stack();
    
    bool Copy(Stack*&);
    bool Copy(Stack**);
    
    bool Push(void*);
    void* Pop();
};

class Queue : public Array
{
public:
    Queue();
    Queue(const Queue&);
    Queue(const Queue*);
    
    ~Queue();
    
    bool Copy(Queue*&);
    bool Copy(Queue**);
    
    bool PushAtEnd(void*);
    void* PopAtFront();
};

class Deque : public Queue
{
public:
    Deque();
    Deque(const Deque&);
    Deque(const Deque*);
    
    bool Copy(Deque*&);
    bool Copy(Deque**);
    
    bool PushAtHead(void*);
    void* PopAtEnd();
};

class List
{
    struct Node
    {
        void* m_pData;

        Node* m_pNext;
        Node* m_pPrev;
    };
    Node* m_pHead;
    Node* m_pTail;

    unsigned m_Length;

public:
    List();
    List(void*);
    ~List();

    void*  operator[](unsigned);

    bool InsertAtEnd(void*);
    bool InsertAtHead(void*);
    bool IsEmpty();
    bool DeleteObject(void*, bool IsDeleteAll = true);
    bool DeleteObjectAtIndex(unsigned);

    bool Copy(List*&);
    bool Copy(List**);
    void Log();

    void* GetObjectAtIndex(unsigned);
    unsigned GetLength();

    void Clear();
    void Release(bool IsOnStack = false);
};

#endif
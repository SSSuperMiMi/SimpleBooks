//
//  TouchMsgCenter.h
//  Simple Books
//
//  Created by Allen Zhao on 5/16/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//
#include "DataStructure.h"
#include "Global.h"

Array::Array() : m_pData(NULL), 
m_current_memory_alloc_length(0),
m_current_length(0),
m_unit_length(sizeof_addr)
{
}

Array::Array(const Array* Src):m_unit_length(sizeof_addr)
{
    if(Src->m_current_length == 0)return;
    unsigned mm_length = Src->m_current_memory_alloc_length;
    m_pData = (void**)malloc(mm_length);
    memcpy(m_pData, Src->m_pData, mm_length);
    m_current_memory_alloc_length = mm_length;
    m_current_length = Src->m_current_length;
}

Array::Array(const Array& Src):m_unit_length(sizeof_addr)
{
    if(Src.m_current_length == 0)return;
    unsigned mm_length = Src.m_current_memory_alloc_length;
    m_pData = (void**)malloc(mm_length);
    memcpy(m_pData, Src.m_pData, mm_length);
    m_current_memory_alloc_length = mm_length;
    m_current_length = Src.m_current_length;
}

void Array::Log()
{
    printf("\n数组%p信息数据信息如下:\n", this);
    for(int i = 0; i < m_current_length; ++i)
    {
        if(i % 6 == 0 && i != 0)printf("\n");
        printf("%p   ", (*this)[i]);
    }
    printf("\n");
}

void Array::Clear()
{
	if (m_pData == NULL)return;
	free((void*)m_pData);
	m_pData = NULL;
	m_current_memory_alloc_length = m_current_length = 0;
}

Array::~Array()
{
	Clear();
}

bool Array::AppendData(void* pData)
{
    return InsertAtEnd(pData);
}

bool Array::Copy(Array*& Obj)
{
    return Obj = new Array(this);
}

bool Array::Copy(Array** Obj)
{
	if (Obj == NULL)return false;
	return *Obj = new Array(this);
}

bool Array::DeleteObjectAtIndex(unsigned index)
{
	if (index > m_current_length - 1 || m_pData == NULL)return false;
	return DeleteObject(m_pData[index]);
}

unsigned Array::GetIndex(void* obj)
{
    for (int i = 0; i < m_current_length; ++i)
    {
        if (obj == m_pData[i])
        {
            return i;
        }
    }
    
    return 0xffffffff;
}

bool Array::Find(void* obj)
{
    for (int i = 0; i < m_current_length; ++i)
    {
        if (obj == m_pData[i])
        {
            return true;
        }
    }
    
    return false;
}

bool Array::DeleteObject(void* pObject)
{
	if (m_pData == NULL)return false;
	bool ret = false;
	for (int i = 0; i < m_current_length; ++i)
	{
		if (pObject == m_pData[i])
		{
			ret = true;
			if (i != m_current_length - 1)
			{
				memmove(
					&m_pData[i], 
					&m_pData[i + 1], 
					(m_current_length - i - 1) * sizeof_addr);
			}
			--m_current_length;
            --i;
            m_pData[m_current_length] = 0;
		}
	}

    if(m_current_length == 0)
    {
        m_current_memory_alloc_length = 0;
        free((void*)m_pData);
        m_pData = nullptr;
        return ret;
    }
    if(m_current_length * sizeof_addr * 2 <= m_current_memory_alloc_length)
    {
        void** pNewData = (void**)malloc(m_current_length * sizeof_addr);
        m_current_memory_alloc_length = m_current_length * sizeof_addr;
        memcpy(pNewData, m_pData, m_current_length * sizeof_addr);
        free((void*)m_pData);
        m_pData = pNewData;
    }
    
	return ret;
}

unsigned Array::GetLength()
{ 
	return m_current_length; 
}

bool Array::IsEmpty()
{
    return m_current_length == 0;
}

bool Array::InsertAtHead(void* pData)
{
	if (m_pData == NULL)
	{
		return InsertAtEnd(pData);
	}
	if (m_current_length * sizeof_addr == m_current_memory_alloc_length)
	{
		if (m_current_memory_alloc_length * 2 > max_length)return false;
		void** pNewData = (void**)malloc(m_current_length * 2 * sizeof_addr);
		m_current_memory_alloc_length *= 2;
		memcpy(&pNewData[1], m_pData, m_current_length * sizeof_addr);
		pNewData[0] = pData;
		++m_current_length;
		free((void*)m_pData);
		m_pData = pNewData;
		return true;
	}
	if (m_current_length * 4 < m_current_memory_alloc_length)
	{
		memmove(&m_pData[1], m_pData, m_current_length * sizeof_addr);
		m_pData[0] = pData;
		++m_current_length;
		return true;
	}
    
    return false;
}

void* Array::GetObjectAtIndex(unsigned Index)
{
    return (*this)[Index];
}

void*  Array::operator[](unsigned Index)const
{
	if (m_pData == NULL || Index > m_current_length - 1)return NULL;
	return m_pData[Index];
}

bool Array::InsertAtEnd(void* pData)
{
	if (m_pData == NULL)
	{
		m_pData = (void**)malloc(sizeof_addr);
		memcpy(m_pData, &pData, sizeof_addr);
		 m_current_memory_alloc_length = sizeof_addr;
		 ++m_current_length;
		return true; 
	}
	if (m_current_length * sizeof_addr == m_current_memory_alloc_length)
	{
		if (m_current_memory_alloc_length * 2 > max_length)return false;
		void** pNewData = (void**)malloc(m_current_memory_alloc_length * 2);
		m_current_memory_alloc_length *= 2;
		memcpy(pNewData, m_pData, m_current_length * sizeof_addr);
		pNewData[m_current_length] = pData;
		++m_current_length;
		free((void*)m_pData);
		m_pData = pNewData;
		return true;
	}
	if (m_current_length * sizeof_addr < m_current_memory_alloc_length)
	{
		m_pData[m_current_length] = pData;
		++m_current_length;
		return true;
	}

	return false;
}

bool Stack::Push(void* pData)
{
    return InsertAtEnd(pData);
}

void* Stack::Pop()
{
    if(m_current_length < 1)return nullptr;
    void* ret = (*this)[m_current_length - 1];
    DeleteObjectAtIndex(m_current_length - 1);
    return  ret;
}

bool Stack::Copy(Stack*& Obj)
{
    return Obj = new Stack(this);
}

bool Stack::Copy(Stack** Obj)
{
    if (Obj == NULL)return false;
    return *Obj = new Stack(this);
}

Stack::~Stack()
{
    
}

Stack::Stack(const Stack& Src):Array(Src)
{
}

Stack::Stack(const Stack* Src):Array(Src)
{
}

Stack::Stack()
{
}

bool Queue::PushAtEnd(void* pData)
{
    return InsertAtEnd(pData);
}

void* Queue::PopAtFront()
{
    if(m_current_length < 1)return nullptr;
    void* ret = (*this)[0];
    DeleteObjectAtIndex(0);
    return  ret;
}

bool Queue::Copy(Queue*& Obj)
{
    return Obj = new Queue(this);
}

bool Queue::Copy(Queue** Obj)
{
    if (Obj == NULL)return false;
    return *Obj = new Queue(this);
}

Queue::~Queue()
{
}

Queue::Queue(const Queue& Src):Array(Src)
{
}

Queue::Queue(const Queue* Src):Array(Src)
{
}

Queue::Queue()
{
}

Deque::Deque()
{
}

Deque::Deque(const Deque& Src):Queue(Src)
{
}

Deque::Deque(const Deque* Src):Queue(Src)
{
}

bool Deque::Copy(Deque*& Obj)
{
    return Obj = new Deque(this);
}

bool Deque::Copy(Deque** Obj)
{
    if (Obj == NULL)return false;
    return *Obj = new Deque(this);
}

bool Deque::PushAtHead(void* pData)
{
    return InsertAtHead(pData);
}

void* Deque::PopAtEnd()
{
    if(m_current_length < 1)return nullptr;
    void* ret = (*this)[m_current_length - 1];
    DeleteObjectAtIndex(m_current_length - 1);
    return  ret;
}

List::List() :m_pHead(NULL), m_pTail(NULL), m_Length(0)
{
}

List::List(void* pData) : m_pHead(NULL), m_pTail(NULL), m_Length(0)
{
	++m_Length;

	void* pNewNode = malloc(sizeof(struct Node));
    memset(pNewNode, 0, sizeof(struct Node));
	m_pHead = m_pTail = (Node*)pNewNode;
	m_pHead->m_pData = pData;
}

List::~List()
{
	while (m_pHead != NULL)
	{
		Node* pDelNode = m_pHead;
		m_pHead = m_pHead->m_pNext;
		free((void*)pDelNode);
	}
}

void*  List::operator[](unsigned Index)
{
	return GetObjectAtIndex(Index);
}

bool List::InsertAtEnd(void* pData)
{
	++m_Length;
	Node* pNewNode = (Node*)malloc(sizeof(struct Node));
    memset(pNewNode, 0, sizeof(struct Node));
	pNewNode->m_pData = pData;
	pNewNode->m_pPrev = m_pTail;

	if (m_pHead == NULL)
	{
		m_pHead = m_pTail = pNewNode;
	}
	else
	{
		m_pTail->m_pNext = pNewNode;	
		m_pTail = pNewNode;
	}

	return true;
}

bool List::IsEmpty()
{
    return m_Length == 0;
}

bool List::InsertAtHead(void* pData)
{
	++m_Length;
	Node* pNewNode = (Node*)malloc(sizeof(struct Node));
    memset(pNewNode, 0, sizeof(struct Node));
	pNewNode->m_pData = pData;
	pNewNode->m_pNext = m_pHead;

	if (m_pHead == NULL)
	{
		m_pHead = m_pTail = pNewNode;
	}
	else
	{
		m_pHead->m_pPrev = pNewNode;
		m_pHead = pNewNode;
	}

	return true;
}

bool List::DeleteObject(void* pData, bool IsDeleteAll)
{
	Node* pNode = m_pHead;
	bool ret = false;
	while (pNode != NULL)
	{
		Node* pDelNode = pNode;
		if (pDelNode->m_pData == pData)
		{
			ret = true;
			if (pDelNode == m_pHead)
			{
				m_pHead = pNode->m_pNext;
				if (m_pHead != NULL)
				{
					m_pHead->m_pPrev = NULL;
				}
				else
				{
					m_pTail = NULL;
				}
			}
			else if (pDelNode == m_pTail)
			{
				m_pTail = m_pTail->m_pPrev;
				if (m_pTail != NULL)
				{
					m_pTail->m_pNext = NULL;
				}
				else
				{
					m_pHead = NULL;
				}
			}
			else
			{
				pDelNode->m_pPrev->m_pNext = pDelNode->m_pNext;
				pDelNode->m_pNext->m_pPrev = pDelNode->m_pPrev;
			}
			pNode = pNode->m_pNext;
			free((void*)pDelNode);
			--m_Length;

			if (IsDeleteAll == false)return ret;
		}
		else
		{
			pNode = pNode->m_pNext;
		}
	}

	return ret;
}

bool List::DeleteObjectAtIndex(unsigned Index)
{
	if (Index > m_Length - 1 || m_Length == 0)return false;
	Node* pNode = m_pHead;
	int i = 0;
	while (true)
	{
		if (i == Index)
		{
			return DeleteObject(pNode->m_pData, false);
		}
		++i;
		pNode = pNode->m_pNext;
	}
}

void List::Log()
{
    printf("\n链表%p信息数据信息如下:\n", this);
    for(int i = 0; i < m_Length; ++i)
    {
        if(i % 6 == 0 && i != 0)printf("\n");
        printf("%p   ", (*this)[i]);
    }
    printf("\n");
}

bool List::Copy(List*& Obj)
{
	Obj = new List();
	for (int i = 0; i < m_Length; ++i)
	{
		Obj->InsertAtEnd(GetObjectAtIndex(i));
	}

	return true;
}

bool List::Copy(List** Obj)
{
	if (Obj == NULL)return false;
	*Obj = new List();
	for (int i = 0; i < m_Length; ++i)
	{
		(*Obj)->InsertAtEnd(GetObjectAtIndex(i));
	}

	return true;
}

void* List::GetObjectAtIndex(unsigned Index)
{
	if (Index > m_Length - 1)return NULL;
	Node* pNode = m_pHead;
	int i = 0;
	while (true)
	{
		if (i == Index)
		{
			return pNode->m_pData;
		}
		++i;
		pNode = pNode->m_pNext;
	}
}

unsigned List::GetLength()
{
	return m_Length;
}

void List::Clear()
{
	int l = m_Length;
	for (int i = 0; i < l; ++i)
	{
		DeleteObjectAtIndex(0);
	}
}

void List::Release(bool IsOnStack)
{
	if (IsOnStack == true)return;
	delete this;
}

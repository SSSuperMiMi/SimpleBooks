//
//  MemoryPool.cpp
//  Simple Books
//
//  Created by Allen Zhao on 5/28/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#include "MemoryPool.h"
#import "Math.h"
#import "Macro.h"

MemoryPool global_memory_pool;
bool MemoryPool::GetMemory(unsigned length, void** ptr)
{
    if (length == 0) {
        *ptr = nullptr;return false;
    }
    if (!(length > max_mm_block_size || unuse_blocks.IsEmpty())) {
        length = math::ksNextPot(length);
        if (length < min_mm_block_size) {
            length = min_mm_block_size;
        }
        
        for (int i = 0; i < unuse_blocks.GetLength(); ++i) {
            block* _block = (block*)unuse_blocks.GetObjectAtIndex(i);
            if (_block->length == length) {
                *ptr = _block->addr;
                unuse_blocks.DeleteObject(_block);
                use_blocks.InsertAtEnd(_block);
                
                return true;
            }
        }
    }
    
    void* mm = malloc(length);
    memset(mm, 0, length);
    block* mm_block = (block*)malloc(sizeof(struct block));
    mm_block->addr = mm;
    mm_block->length = length;
    extra_blocks.InsertAtEnd(mm_block);
    return *ptr = mm;
}

bool MemoryPool::ReleaseMemory(void* ptr)
{
    if (ptr == nullptr) {
        return false;
    }

    for (int i = 0; i < use_blocks.GetLength(); ++i) {
        block* _block = (block*)use_blocks.GetObjectAtIndex(i);
        if (_block->addr == ptr) {
            use_blocks.DeleteObject(_block);
            unuse_blocks.InsertAtEnd(_block);
            
            return true;
        }
    }
    
    for (int i = 0; i < extra_blocks.GetLength(); ++i) {
        block* _block = (block*)extra_blocks.GetObjectAtIndex(i);
        if (_block->addr == ptr) {
            extra_blocks.DeleteObject(_block);
            free(_block);
            free(ptr);
            
            return true;
        }
    }
    
    return false;
}

MemoryPool::MemoryPool():base_address(NULL), max_mm_block_size(0x4000), min_mm_block_size(0x80)
{
    unsigned size = min_mm_block_size, num = min_mm_block_size;
    unsigned mm_length = 0;
    while (num)
    {
        mm_length += size* num;
        
        size <<= 1;
        num  >>= 1;
    }
    size = min_mm_block_size;
    num  = min_mm_block_size;
    
    base_address = malloc(mm_length);
    memset(base_address, 0, mm_length);
    unsigned char* ptr = (unsigned char*)base_address;
    while (num) {
        for (int i = 0; i < num; ++i) {
            block* mm_block = (block*)malloc(sizeof(struct block));
            mm_block->addr = ptr;
            mm_block->length = size;
            unuse_blocks.InsertAtEnd(mm_block);
            ptr += size;
        }
        
        size <<= 1;
        num  >>= 1;
    }
}

MemoryPool::~MemoryPool()
{
    free(base_address);
    
    for (int i = 0; i < use_blocks.GetLength(); ++i)
    {
        free((void*)use_blocks.GetObjectAtIndex(i));
    }
    for (int i = 0; i < unuse_blocks.GetLength(); ++i)
    {
        free((void*)unuse_blocks.GetObjectAtIndex(i));
    }
    
    for (int i = 0; i < extra_blocks.GetLength(); ++i) {
        free(((block*)unuse_blocks.GetObjectAtIndex(i))->addr);
        free((void*)unuse_blocks.GetObjectAtIndex(i));
    }
}
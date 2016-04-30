//
//  MemoryPool.h
//  Simple Books
//
//  Created by Allen Zhao on 5/28/15.
//  Copyright (c) 2015 Apple Orchard. All rights reserved.
//

#ifndef __Simple_Books__MemoryPool__
#define __Simple_Books__MemoryPool__

#include <stdio.h>
#include "DataStructure.h"

class MemoryPool {
    struct block
    {
        void* addr;
        unsigned length;
    };
    
public:
    MemoryPool();
    ~MemoryPool();
    
    List use_blocks;
    List unuse_blocks;
    List extra_blocks;
    
    unsigned max_mm_block_size;
    unsigned min_mm_block_size;
    
    void* base_address;
    
    bool GetMemory(unsigned, void**);
    bool ReleaseMemory(void*);
};

extern MemoryPool global_memory_pool;
#endif /* defined(__Simple_Books__MemoryPool__) */


#ifndef __TRACE_MEM_H__
#define __TRACE_MEM_H__

#include <stdlib.h>
#include <stdint.h>

typedef struct __attribute__((packed, aligned(1))) trace_memblock{
    uint8_t* mem_address;
    size_t block_size;
    
    struct trace_memblock* next;
}trace_mem_block_t;

typedef struct __attribute__((packed, aligned(1))) trace_mempool{
    uint8_t* mem_address;
    uint8_t* current_address_ptr;

    trace_mem_block_t* mem_block_member;
    uint32_t block_number;

    size_t total_size;
    size_t used_size;
}trace_mem_t;

void trace_mem_init(uint8_t* address_head, size_t mem_size);

void* trace_malloc(size_t size);

void trace_free(int* address);

#endif
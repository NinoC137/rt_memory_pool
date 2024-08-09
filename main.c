#include "stdio.h"
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

static trace_mem_t trace_mem;

void trace_mem_init(uint8_t* address_head, size_t mem_size){
    trace_mem.mem_address = address_head;
    trace_mem.current_address_ptr = address_head;

    trace_mem.mem_block_member = (trace_mem_block_t*)address_head;
    trace_mem.mem_block_member->mem_address = address_head;
    trace_mem.mem_block_member->block_size = 0;
    trace_mem.mem_block_member->next = (trace_mem_block_t*)address_head;
    trace_mem.block_number = 0;

    trace_mem.total_size = mem_size;
    trace_mem.used_size = 0;
}

void* trace_malloc(size_t size){
    trace_mem.block_number++;
    
    trace_mem.mem_block_member = (trace_mem_block_t*)trace_mem.current_address_ptr;
    trace_mem.mem_block_member->mem_address = trace_mem.current_address_ptr;
    trace_mem.mem_block_member->block_size = size;
    trace_mem.mem_block_member->next = (trace_mem_block_t*)(trace_mem.current_address_ptr + size);

    trace_mem.current_address_ptr += size;
    trace_mem.used_size += size;

    return trace_mem.mem_block_member->mem_address;
}

void trace_free(int* address){
    trace_mem_block_t* block_ptr = (trace_mem_block_t*)address;
    trace_mem.used_size -= block_ptr->block_size;
    while(block_ptr->next != NULL){
        block_ptr->next->mem_address -= block_ptr->block_size;
        block_ptr->next->next -= block_ptr->block_size;
        block_ptr = (block_ptr->next + block_ptr->block_size);
    }
}


static uint8_t trace_mempool[1024 * 1024 * 32];    //32MiB

int main(int argc, char* argv[])
{
    trace_mem_init(trace_mempool, sizeof(trace_mempool));
    int* test_mem_1024 = (int*)trace_malloc(1024);
    
    printf("trace_malloc memory address: %p\r\n", test_mem_1024);

    trace_mem_block_t* block_ptr = (trace_mem_block_t*)test_mem_1024;
    
    printf("mem start address: %p\r\n", block_ptr->mem_address);
    printf("mem size: %d\r\n", block_ptr->block_size);
    printf("next block address: %p\r\n", block_ptr->next);

    int* test2_mem_1024 = (int*)trace_malloc(1024);
    printf("trace_malloc 2 memory address: %p\r\n", test2_mem_1024);

    block_ptr = (trace_mem_block_t*)test2_mem_1024;
    
    printf("mem2 start address: %p\r\n", block_ptr->mem_address);
    printf("mem2 size: %d\r\n", block_ptr->block_size);
    printf("next block2 address: %p\r\n", block_ptr->next);

    return 0;
}
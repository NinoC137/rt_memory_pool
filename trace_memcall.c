#include "trace_memcall.h"

static trace_mem_t trace_mem;

void trace_mem_init(uint8_t* address_head, size_t mem_size){
    trace_mem.mem_address = address_head;
    trace_mem.current_address_ptr = address_head;

    trace_mem.mem_block_member->mem_address = address_head;
    trace_mem.mem_block_member->block_size = 0;
    trace_mem.mem_block_member->next = (trace_mem_block_t*)address_head;
    trace_mem.block_number = 0;

    trace_mem.total_size = mem_size;
    trace_mem.used_size = 0;
}

void* trace_malloc(size_t size){
    trace_mem.block_number++;
    
    trace_mem.mem_block_member->mem_address = trace_mem.current_address_ptr + trace_mem.used_size;
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

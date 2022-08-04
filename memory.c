#include <stdio.h>
#include "oslabs.h"
#include <string.h>
#include <stdbool.h>


/*
 struct MEMORY_BLOCK {
        int start_address;
        int end_address;
        int segment_size;
        int process_id; //0 indicates a free block
    };
*/

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    int index = 0;
    int best_fit_size = 0;
    bool is_match = false;
    for (int i = 0; i < *map_cnt; i++) {
        if ((memory_map[i].segment_size == request_size) && (memory_map[i].process_id == 0)) {
            memory_map[i].process_id = process_id;
            return memory_map[i];
        }
        else if ((memory_map[i].segment_size > request_size) && (memory_map[i].process_id == 0)) {
            if (is_match == false) {
                best_fit_size = memory_map[i].segment_size;
                index = i;
                is_match = true;
            }
            else if (memory_map[i].segment_size < best_fit_size) {
                    best_fit_size = memory_map[i].segment_size;
                    index = i;
            }
        }
    }
    struct MEMORY_BLOCK allocated_memory;
    allocated_memory.start_address = 0;
    allocated_memory.end_address = 0;
    allocated_memory.segment_size = 0;
    allocated_memory.process_id = 0;
    if (is_match == true) {
        allocated_memory.start_address = memory_map[index].start_address;
        allocated_memory.end_address = allocated_memory.start_address + (request_size - 1);
        allocated_memory.segment_size = request_size;
        allocated_memory.process_id = process_id;
        memory_map[index].start_address = allocated_memory.end_address + 1;
        memory_map[index].segment_size = memory_map[index].end_address - memory_map[index].start_address + 1;
        for (int i = *map_cnt; i > index; i--){
            memory_map[i] = memory_map[i-1];
        }
        memory_map[index] = allocated_memory;
        (*map_cnt)++;
    }
    return allocated_memory;
}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    int index = 0;
    int first_fit_size = 0;
    bool is_match = false;
    for (int i = 0; i < *map_cnt; i++) {
        if ((memory_map[i].segment_size >= request_size) && (memory_map[i].process_id == 0)) {
            if (memory_map[i].segment_size == request_size) {
                memory_map[i].process_id = process_id;
                return memory_map[i];
            }
            else {
                first_fit_size = memory_map[i].segment_size;
                index = i;
                is_match = true;
                break;
            }
        }
    }
    struct MEMORY_BLOCK allocated_memory;
    allocated_memory.start_address = 0;
    allocated_memory.end_address = 0;
    allocated_memory.segment_size = 0;
    allocated_memory.process_id = 0;
    if (is_match == true) {
        allocated_memory.start_address = memory_map[index].start_address;
        allocated_memory.end_address = allocated_memory.start_address + (request_size - 1);
        allocated_memory.segment_size = request_size;
        allocated_memory.process_id = process_id;
        memory_map[index].start_address = allocated_memory.end_address + 1;
        memory_map[index].segment_size = memory_map[index].end_address - memory_map[index].start_address + 1;
        for (int i = *map_cnt; i > index; i--){
            memory_map[i] = memory_map[i-1];
        }
        memory_map[index] = allocated_memory;
        (*map_cnt)++;
    }
    return allocated_memory;
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    int index = 0;
    int worst_fit_size = 0;
    bool is_match = false;
    for (int i = 0; i < *map_cnt; i++) {
        if ((memory_map[i].segment_size >= request_size) && (memory_map[i].process_id == 0)) {
            if (is_match == false) {
                worst_fit_size = memory_map[i].segment_size;
                index = i;
                is_match = true;
            }
            else {
                if (memory_map[i].segment_size > worst_fit_size) {
                    worst_fit_size = memory_map[i].segment_size;
                    index = i;
                }
            }
        }
    }
    if ((memory_map[index].segment_size == request_size) && (is_match == true)) {
        memory_map[index].process_id = process_id;
        return memory_map[index]; 
    }
    struct MEMORY_BLOCK allocated_memory;
    allocated_memory.start_address = 0;
    allocated_memory.end_address = 0;
    allocated_memory.segment_size = 0;
    allocated_memory.process_id = 0;
    if (is_match == true) {
        allocated_memory.start_address = memory_map[index].start_address;
        allocated_memory.end_address = allocated_memory.start_address + (request_size - 1);
        allocated_memory.segment_size = request_size;
        allocated_memory.process_id = process_id;
        memory_map[index].start_address = allocated_memory.end_address + 1;
        memory_map[index].segment_size = memory_map[index].end_address - memory_map[index].start_address + 1;
        for (int i = *map_cnt; i > index; i--){
            memory_map[i] = memory_map[i-1];
        }
        memory_map[index] = allocated_memory;
        (*map_cnt)++;
    }
    return allocated_memory;
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address) {
    int index = 0;
    int next_fit_size = 0;
    int last_index = 0;
    bool is_match = false;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].start_address == last_address) {
            last_index = i;
        }
    }
    for (int i = last_index; i < *map_cnt; i++) {
        if ((memory_map[i].segment_size >= request_size) && (memory_map[i].process_id == 0)) {
            if (memory_map[i].segment_size == request_size) {
                memory_map[i].process_id = process_id;
                return memory_map[i];
            }
            else {
                next_fit_size = memory_map[i].segment_size;
                index = i;
                is_match = true;
                break;
            }
        }
    }
    struct MEMORY_BLOCK allocated_memory;
    allocated_memory.start_address = 0;
    allocated_memory.end_address = 0;
    allocated_memory.segment_size = 0;
    allocated_memory.process_id = 0;
    if (is_match == true) {
        allocated_memory.start_address = memory_map[index].start_address;
        allocated_memory.end_address = allocated_memory.start_address + (request_size - 1);
        allocated_memory.segment_size = request_size;
        allocated_memory.process_id = process_id;
        memory_map[index].start_address = allocated_memory.end_address + 1;
        memory_map[index].segment_size = memory_map[index].end_address - memory_map[index].start_address + 1;
        for (int i = *map_cnt; i > index; i--){
            memory_map[i] = memory_map[i-1];
        }
        memory_map[index] = allocated_memory;
        (*map_cnt)++;
    }
    return allocated_memory;
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt) {
    if ((*map_cnt == 1) && (memory_map[0].start_address == 0) && (memory_map[0].end_address == 0) && (memory_map[0].segment_size == 0) && (memory_map[0].process_id == 0)) {
        return;
    }
    else {
        for (int i = 0; i < *map_cnt; i++) {
            if ((freed_block.start_address == memory_map[i].start_address) && (freed_block.end_address == memory_map[i].end_address) && (freed_block.segment_size == memory_map[i].segment_size) && (freed_block.process_id == memory_map[i].process_id)) {
                memory_map[i].process_id = 0;
                if (i < *map_cnt - 1){
                    if (memory_map[i + 1].process_id == 0) {
                        memory_map[i].end_address = memory_map[i + 1].end_address;
                        memory_map[i].segment_size = memory_map[i].segment_size + memory_map[i + 1].segment_size;
                        for (int k = i + 1; k < *map_cnt - 1; k++){
                            memory_map[k] = memory_map[k + 1];
                        }
                        (*map_cnt)--;
                    }
                }
                if (i > 0) {
                    if (memory_map[i - 1].process_id == 0) {
                        memory_map[i - 1].end_address = memory_map[i].end_address;
                        memory_map[i - 1].segment_size = memory_map[i - 1].segment_size + memory_map[i].segment_size;
                        for (int k = i; k < *map_cnt - 1; k++) {
                            memory_map[k] = memory_map[k + 1];
                        }
                        (*map_cnt)--;
                    }
                }
            }
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include "oslabs.h"

/*
struct PCB {
        int process_id;
        int arrival_timestamp;
        int total_bursttime;
        int execution_starttime;
        int execution_endtime;
        int remaining_bursttime;
        int process_priority;
};
*/

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (current_process.process_id == 0 && current_process.arrival_timestamp == 0 && current_process.total_bursttime == 0 && current_process.execution_starttime == 0 && current_process.execution_endtime == 0 && current_process.remaining_bursttime == 0 && current_process.process_priority == 0) {
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            return new_process;
        }
    if (new_process.process_priority >= current_process.process_priority) {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt = *queue_cnt + 1;
        return current_process;
        }
    else {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        current_process.remaining_bursttime = current_process.execution_endtime - timestamp;
        current_process.execution_endtime = 0;
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt = *queue_cnt + 1;
        return new_process;
        }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        struct PCB nullpcb;
        nullpcb.process_id = 0;
        nullpcb.arrival_timestamp = 0;
        nullpcb.total_bursttime = 0;
        nullpcb.execution_starttime = 0;
        nullpcb.execution_endtime = 0;
        nullpcb.remaining_bursttime = 0;
        nullpcb.process_priority = 0;
        return nullpcb;
    }
    int highest_priority_integer = ready_queue[0].process_priority;
    int highest_priority_index = 0;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].process_priority < highest_priority_integer) {
            highest_priority_integer = ready_queue[i].process_priority;
            highest_priority_index = i;
        }
    }
    struct PCB new_process = ready_queue[highest_priority_index];
    new_process.execution_starttime = timestamp;
    new_process.execution_endtime = timestamp + new_process.total_bursttime;
    for (int i = highest_priority_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    *queue_cnt = *queue_cnt - 1;
    return new_process;
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp) {
    if (current_process.process_id == 0 && current_process.arrival_timestamp == 0 && current_process.total_bursttime == 0 && current_process.execution_starttime == 0 && current_process.execution_endtime == 0 && current_process.remaining_bursttime == 0 && current_process.process_priority == 0) {
            new_process.execution_starttime = time_stamp;
            new_process.execution_endtime = time_stamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            return new_process;
    }
    if(current_process.remaining_bursttime <= new_process.total_bursttime) {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt = *queue_cnt + 1;
        return current_process;
        }
    else {
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        current_process.remaining_bursttime = current_process.execution_endtime - time_stamp;
        current_process.execution_starttime = 0;
        current_process.execution_endtime = 0;
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt = *queue_cnt + 1;
        return new_process;
        }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        struct PCB nullpcb;
        nullpcb.process_id = 0;
        nullpcb.arrival_timestamp = 0;
        nullpcb.total_bursttime = 0;
        nullpcb.execution_starttime = 0;
        nullpcb.execution_endtime = 0;
        nullpcb.remaining_bursttime = 0;
        nullpcb.process_priority = 0;
        return nullpcb;
    }
    int smallest_rbt_index = 0;
    int smallest_rbt = ready_queue[0].remaining_bursttime;
    for (int i = 1; i < *queue_cnt; i++) {
        if(ready_queue[i].remaining_bursttime < smallest_rbt) {
            smallest_rbt = ready_queue[i].remaining_bursttime;
            smallest_rbt_index = i;
        }
    }
    struct PCB new_process = ready_queue[smallest_rbt_index];
    new_process.execution_starttime = timestamp;
    new_process.execution_endtime = timestamp + new_process.remaining_bursttime;
    for (int i = smallest_rbt_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    *queue_cnt = *queue_cnt - 1;
    return new_process;
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    if (current_process.process_id == 0 && current_process.arrival_timestamp == 0 && current_process.total_bursttime == 0 && current_process.execution_starttime == 0 && current_process.execution_endtime == 0 && current_process.remaining_bursttime == 0 && current_process.process_priority == 0) {
            new_process.execution_starttime = timestamp;
            if(new_process.total_bursttime < time_quantum){
                new_process.execution_endtime = timestamp + new_process.total_bursttime;
            }
            else {
                new_process.execution_endtime = timestamp + time_quantum;
            }
            new_process.remaining_bursttime = new_process.total_bursttime;
            return new_process;
        }
    new_process.execution_starttime = 0;
    new_process.execution_endtime = 0;
    new_process.remaining_bursttime = new_process.total_bursttime;
    ready_queue[*queue_cnt] = new_process;
    *queue_cnt = *queue_cnt + 1;
    return current_process;
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum) {
    if (*queue_cnt == 0) {
        struct PCB nullpcb;
        nullpcb.process_id = 0;
        nullpcb.arrival_timestamp = 0;
        nullpcb.total_bursttime = 0;
        nullpcb.execution_starttime = 0;
        nullpcb.execution_endtime = 0;
        nullpcb.remaining_bursttime = 0;
        nullpcb.process_priority = 0;
        return nullpcb;
    }
    int earliest_task_index = 0;
    int earliest_at = ready_queue[0].arrival_timestamp;
    for (int i = 1; i < *queue_cnt; i++){
        if (ready_queue[i].remaining_bursttime < earliest_at) {
            earliest_at = ready_queue[i].remaining_bursttime;
            earliest_task_index = i;
        }
    }
    struct PCB new_process = ready_queue[earliest_task_index];
    for (int i = earliest_task_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i+1];
    }
    *queue_cnt = *queue_cnt - 1;
    new_process.execution_starttime = time_stamp;
    if (time_quantum < new_process.remaining_bursttime) {
        new_process.execution_endtime = time_stamp + time_quantum;
    }
    else {
        new_process.execution_endtime = time_stamp +
        new_process.remaining_bursttime;
    }
    return new_process;
}

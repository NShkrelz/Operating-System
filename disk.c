#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "oslabs.h"

/*
struct RCB {
int request_id;
int arrival_timestamp;
int cylinder;
int address;
int process_id;
}
*/

struct RCB handle_request_arrival_fcfs (struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if ((current_request.request_id == 0) && (current_request.arrival_timestamp == 0) && (current_request.cylinder == 0) && (current_request.address == 0) && (current_request.process_id == 0)) {
        return new_request;
    }
    else {
        if (*queue_cnt < QUEUEMAX) {
            new_request.arrival_timestamp = timestamp;
            request_queue[*queue_cnt] = new_request;
            (*queue_cnt)++;
            return current_request;
        }
        else {
            return current_request;
        }
    }
}

struct RCB handle_request_completion_fcfs (struct RCB request_queue[QUEUEMAX], int *queue_cnt) {
    if (*queue_cnt == 0) {
        struct RCB null_RCB;
        null_RCB.request_id = 0;
        null_RCB.arrival_timestamp = 0;
        null_RCB.cylinder = 0;
        null_RCB.address = 0;
        null_RCB.process_id = 0;
        return null_RCB;
    }
    else {
        int next_index = 0;
        int first_arrival = request_queue[0].arrival_timestamp;
        for (int i = 1; i < *queue_cnt; i++){
            if (first_arrival > request_queue[i].arrival_timestamp) {
                first_arrival = request_queue[i].arrival_timestamp;
                next_index = i;
            }
        }
        struct RCB next_RCB = request_queue[next_index];
        for (int i = next_index; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        return next_RCB;
    }
}

struct RCB handle_request_arrival_sstf (struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if ((current_request.request_id == 0) && (current_request.arrival_timestamp == 0) && (current_request.cylinder == 0) && (current_request.address == 0) && (current_request.process_id == 0)) {
        return new_request;
    }
    else {
        if (*queue_cnt < QUEUEMAX) {
            request_queue[*queue_cnt] = new_request;
            (*queue_cnt)++;
            return current_request;
        }
        else {
            return current_request;
        }
    }
}

struct RCB handle_request_completion_sstf (struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder) {
    if (*queue_cnt == 0) {
        struct RCB null_RCB;
        null_RCB.request_id = 0;
        null_RCB.arrival_timestamp = 0;
        null_RCB.cylinder = 0;
        null_RCB.address = 0;
        null_RCB.process_id = 0;
        return null_RCB;
    }
    else {
        int next_index = 0;
        int closest_cyl = 0;
        int earliest_arrival = 0;
        closest_cyl = abs(current_cylinder - request_queue[0].cylinder);
        earliest_arrival = request_queue[0].arrival_timestamp;
        for (int i = 1; i < *queue_cnt; i++) {
            if (closest_cyl == abs(current_cylinder - request_queue[i].cylinder)) {
                if (earliest_arrival > request_queue[i].arrival_timestamp) {
                    closest_cyl = abs(current_cylinder - request_queue[i].cylinder);
                    earliest_arrival = request_queue[i].arrival_timestamp;
                    next_index = i;
                }
            }
            else if (closest_cyl > abs(current_cylinder - request_queue[i].cylinder)) {
                closest_cyl = abs(current_cylinder - request_queue[i].cylinder);
                earliest_arrival = request_queue[i].arrival_timestamp;
                next_index = i;
            }
        }
        struct RCB next_RCB = request_queue[next_index];
        for (int i = next_index; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        return next_RCB;
    }
}

struct RCB handle_request_arrival_look (struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if ((current_request.request_id == 0) && (current_request.arrival_timestamp == 0) && (current_request.cylinder == 0) && (current_request.address == 0) && (current_request.process_id == 0)) {
        return new_request;
    }
    else {
        if (*queue_cnt < QUEUEMAX) {
            request_queue[*queue_cnt] = new_request;
            (*queue_cnt) ++;
            return current_request;
        }
        else {
            return current_request;
        }
    }
}

struct RCB handle_request_completion_look (struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction) {
    if (*queue_cnt == 0) {
        null_RCB.request_id = 0;
        null_RCB.arrival_timestamp = 0;
        null_RCB.cylinder = 0;
        null_RCB.address = 0;
        null_RCB.process_id = 0;
        return null_RCB;
    }
    else {
        int next_index = 0;
        int closest_cyl = 0;
        int earliest_arrivel = 0;
        bool same_cyl = false;
        bool positive_cylinder = false;
        bool direction_first = false;
        bool opposite_direction_first = false;
        for (int i = 0; i < *queue_cnt; i++){
            if (current_cylinder == request_queue[i].cylinder) {
                if (same_cyl == false) {
                    earliest_arrivel = request_queue[i].arrival_timestamp;
                    same_cyl = true;
                    next_index = i;
                }
                else {
                    if (earliest_arrivel > request_queue[i].arrival_timestamp) {
                        earliest_arrivel = request_queue[i].arrival_timestamp;
                        next_index = i;
                    }
                }
            }
            else if ((scan_direction == 1) && (same_cyl == false)) {
                if (request_queue[i].cylinder - current_cylinder > 0) {
                    if (direction_first == false) {
                        closest_cyl = request_queue[i].cylinder - current_cylinder;
                        next_index = i;
                        positive_cylinder = true;
                        direction_first = true;
                    }
                    else {
                        if (closest_cyl > request_queue[i].cylinder - current_cylinder) {
                        closest_cyl = request_queue[i].cylinder - current_cylinder;
                        next_index = i;
                        }
                    }
                }
                else if ((positive_cylinder == false) && (current_cylinder - request_queue[i].cylinder > 0)) {
                    if (opposite_direction_first == false) {
                        closest_cyl = abs(request_queue[i].cylinder - current_cylinder);
                        next_index = i;
                        opposite_direction_first = true;
                    }
                    else if (closest_cyl > abs(request_queue[i].cylinder - current_cylinder)) {
                        closest_cyl = abs(request_queue[i].cylinder - current_cylinder);
                        next_index = i;
                    }
                }
            }
            else if ((scan_direction == 0) && (same_cyl == false)) {
                if (current_cylinder - request_queue[i].cylinder > 0) {
                    if (direction_first == false) {
                        closest_cyl = current_cylinder - request_queue[i].cylinder;
                        next_index = i;
                        positive_cylinder = true;
                        direction_first = true;
                    }
                    else if (closest_cyl > current_cylinder - request_queue[i].cylinder) {
                        closest_cyl = current_cylinder - request_queue[i].cylinder;
                        next_index = i;
                    }
                }
                else if ((positive_cylinder == false) && (current_cylinder - request_queue[i].cylinder < 0)) {
                    if (opposite_direction_first == false) {
                        closest_cyl = abs(current_cylinder - request_queue[i].cylinder);
                        next_index = i;
                        opposite_direction_first = true;
                    }
                    else if (closest_cyl > abs(request_queue[i].cylinder - current_cylinder)) {
                        closest_cyl = abs(current_cylinder - request_queue[i].cylinder);
                        next_index = i;
                    }
                }
            }
        }
        struct RCB next_RCB = request_queue[next_index];
        for (int i = next_index; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        return next_RCB;
    }
}


#include "oslabs.h"

int isNullPCB(struct PCB newProcess);
struct PCB createNullPCB();
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp);
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp);
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum);
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum);

int isNullPCB(struct PCB newProcess)
{

    if ((newProcess.process_id == 0) && (newProcess.arrival_timestamp == 0) && (newProcess.total_bursttime == 0) && (newProcess.execution_starttime == 0) && (newProcess.execution_endtime == 0) && (newProcess.remaining_bursttime == 0) && (newProcess.process_priority == 0))
    {
        return 1;
    }

    return 0;
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{

    if (!isNullPCB(current_process))
    {
        if (new_process.process_priority >= current_process.process_priority)
        {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = new_process;
            *queue_cnt = *queue_cnt + 1;

            return current_process;
        }
        else
        {
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            current_process.execution_endtime = 0;
            current_process.remaining_bursttime -= timestamp - current_process.execution_starttime;
            ready_queue[(*queue_cnt)] = current_process;
            *queue_cnt = *queue_cnt + 1;

            return new_process;
        }
    }

    new_process.execution_starttime = timestamp;
    new_process.execution_endtime = timestamp + new_process.total_bursttime;
    new_process.remaining_bursttime = new_process.total_bursttime;

    return new_process;
}

struct PCB createNullPCB()
{
    struct PCB nullPCB;
    nullPCB.process_id = 0;
    nullPCB.arrival_timestamp = 0;
    nullPCB.total_bursttime = 0;
    nullPCB.execution_starttime = 0;
    nullPCB.execution_endtime = 0;
    nullPCB.remaining_bursttime = 0;
    nullPCB.process_priority = 0;
    return nullPCB;
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{
    struct PCB nullPCB;
    struct PCB temp;

    if (*queue_cnt == 0)
    {
        nullPCB = createNullPCB();
        return nullPCB;
    }

    int highest_priority = ready_queue[0].process_priority;
    int hpr = 0; // highest process reference

    for (int x = 0; x < *queue_cnt; x++)
    {
        if (!(highest_priority < ready_queue[x].process_priority))
        {
            highest_priority = ready_queue[x].process_priority;
            hpr = x;
        }
    }

    temp = ready_queue[hpr];
    temp.execution_starttime = timestamp;
    temp.execution_endtime = timestamp + temp.remaining_bursttime;

    for (int x = hpr; x < *queue_cnt - 1; x++)
    {
        ready_queue[x] = ready_queue[x + 1];
    }
    *queue_cnt = *queue_cnt - 1;

    return temp;
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp)
{
    if (!isNullPCB(current_process))
    {
        if (current_process.remaining_bursttime < new_process.total_bursttime)
        {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = new_process;
            (*queue_cnt)++;
            return current_process;
        }
        else
        {
            new_process.execution_starttime = time_stamp;
            new_process.execution_endtime = time_stamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;

            current_process.execution_endtime = 0;
            current_process.remaining_bursttime = current_process.total_bursttime - (time_stamp - current_process.execution_starttime);
            current_process.execution_starttime = 0;
            ready_queue[*queue_cnt] = current_process;
            (*queue_cnt)++;
            return new_process;
        }
    }

    new_process.execution_starttime = time_stamp;
    new_process.execution_endtime = time_stamp + new_process.total_bursttime;
    new_process.remaining_bursttime = new_process.total_bursttime;
    return new_process;
}
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{
    struct PCB nullPCB;
    struct PCB temp;
    if (*queue_cnt == 0)
    {
        nullPCB = createNullPCB();
        return nullPCB;
    }

    int smallest_remaining_burst_time = ready_queue[0].remaining_bursttime;
    int index = 0;

    for (int x = 0; x < *queue_cnt; x++)
    {
        if (ready_queue[x].remaining_bursttime < smallest_remaining_burst_time)
        {
            smallest_remaining_burst_time = ready_queue[x].remaining_bursttime;
            index = x;
        }
    }

    temp = ready_queue[index];
    temp.execution_starttime = timestamp;
    temp.execution_endtime = timestamp + temp.remaining_bursttime;

    for (int x = index; x < *queue_cnt - 1; x++)
    {
        ready_queue[x] = ready_queue[x + 1];
    }

    *queue_cnt = *queue_cnt - 1;
    return temp;
}
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum)
{
    if (!isNullPCB(current_process))
    {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        return current_process;
    }

    new_process.execution_starttime = timestamp;
    if (new_process.total_bursttime < time_quantum)
    {
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
    }
    else
    {
        new_process.execution_endtime = timestamp + time_quantum;
    }
    new_process.remaining_bursttime = new_process.total_bursttime;
    return new_process;
}
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum)
{
    struct PCB nullPCB;
    struct PCB temp;

    if (*queue_cnt == 0)
    {
        nullPCB = createNullPCB();
        return nullPCB;
    }

    int eat = 0;
    int eat_index = 0;

    for (int x = 0; x < *queue_cnt; x++)
    {
        if (ready_queue[x].arrival_timestamp < ready_queue[eat_index].arrival_timestamp)
        {
            eat = ready_queue[x].arrival_timestamp;
            eat_index = x;
        }
    }

    temp = ready_queue[eat_index];
    for (int x = eat_index; x < *queue_cnt - 1; x++)
    {
        ready_queue[x] = ready_queue[x + 1];
    }
    *queue_cnt = *queue_cnt - 1;

    temp.execution_starttime = time_stamp;
    if (temp.remaining_bursttime < time_quantum)
    {
        temp.execution_endtime = time_stamp + temp.remaining_bursttime;
    }
    else
    {
        temp.execution_endtime = time_stamp + time_quantum;
    }

    return temp;
}

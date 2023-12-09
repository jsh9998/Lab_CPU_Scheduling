#include "oslabs.h"

int isNullPCB(struct PCB newProcess)
{

    if (newProcess.process_id == 0 && newProcess.arrival_timestamp == 0 && newProcess.total_bursttime == 0 && newProcess.execution_starttime == 0 && newProcess.execution_endtime == 0 && newProcess.remaining_bursttime == 0 && newProcess.process_priority == 0)
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
            ready_queue[*queue_cnt + 1] = new_process;
            (*queue_cnt)++;
            return current_process;
        }
        else
        {

            current_process.execution_endtime = 0;
            ready_queue[*queue_cnt + 1] = current_process;
            (*queue_cnt)++;

            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            return new_process;
        }
    }

    new_process.execution_starttime = timestamp;
    new_process.execution_endtime = timestamp + new_process.total_bursttime;
    new_process.remaining_bursttime = new_process.total_bursttime;
    return new_process;
}
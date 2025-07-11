#include "../include/linux/sched.h"
#include "../include/linux/task.h"


extern task_t* tasks[NR_TASKS];
extern void switch_task(task_t* task);
extern void switch_idle_task(task_t* task);


task_t* current = NULL;

task_t* find_ready_taask() {
    task_t* next = NULL;

    // 判断是不是所有任务都被调度了一轮
    bool is_all_zero = true;
    bool is_null = true;

    for (int i = 1; i < NR_TASKS; ++i) {
        task_t* task = tasks[i];

        if (NULL == task) continue;

        is_null = false;

        if (0 != task->counter) {
            is_all_zero = false;
            break;
        }
    }

    // 如果没有任务需要调度，is_all_zero也为true,排除这种情况
    if (!is_null && is_all_zero) goto reset_task;

    // 找到待调度的任务
    for (int i = 1; i < NR_TASKS; ++i) {
        task_t* task = tasks[i];

        if (NULL == task) {
            continue;
        }

        if (current == task && TASK_RUNNING == task->state) {
            task->state = TASK_READY;
        }

        if (TASK_READY != task->state) continue;

        if (NULL == next) {
            next = task;
        } else {
            if (task->counter > next->counter) {
                next = task;
            }
        }
    }

    if (NULL == next) {
        next = tasks[0];
    }


    return next;

    /**
     * 如果所有任务的counter都为0,代表所有任务都被调度了一轮
     * 重新赋值
     */
reset_task:
    if (is_all_zero) {
        for (int i = 1; i < NR_TASKS; ++i) {
            task_t* tmp = tasks[i];

            if (NULL == tmp) continue;

            tmp->counter = tmp->priority;
        }

        // 重新设置counter后，再次查找可调度的任务
        return find_ready_taask();
    }
}



void sched() {
    if (NULL != current) {
        if (TASK_SLEEPING != current->state) {
            current->state = TASK_READY;
        }

        current = NULL;
    }

    task_t* next = find_ready_taask();

    next->state = TASK_RUNNING;

    current = next;

    switch_task(next);
}

void do_timer() {
    if (NULL == current) {
        sched();
    }

    if ((NULL != current) && (current->counter > 0)) {
        current->counter--;
        return;
    }
    sched();
}
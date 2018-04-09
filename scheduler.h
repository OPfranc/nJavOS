/**
 * nJavOS - Not Java Operating System
 * by Alunos SOE 2018/1
 */


#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include "types.h"

#define QUANTUM 4

extern ready_Queue_t Ready_Queue;

u_int rr_scheduler();
u_int priority_scheduler();

#endif	/* SCHEDULER_H */


/* 
 * File:   config_timer.h
 * Author: teabr
 *
 * Created on 9 de Abril de 2018, 15:18
 */

#ifndef CONFIG_TIMER_H
#define	CONFIG_TIMER_H

#include <xc.h>
#include "config.h"
#include "scheduler.h"
   
void nJavOS_inti_timer();
void interrupt timer_zero(void);

#endif	/* CONFIG_TIMER_H */


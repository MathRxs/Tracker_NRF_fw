#ifndef TIMER_H_
#define TIMER_H_
// Includes
#include "general_include.h"
extern uint8_t waked_up;
// Variables
// Constants
#define TIMER_TIMEOUT_SEC 10 // 10 minutes
// Prototypes
void timer_init(void);
void timer_start(uint16_t timeout_sec);
void timer_stop(void);
void timer_restart(uint16_t timeout_sec);
#endif /* TIMER_H_ */

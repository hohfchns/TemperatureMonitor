
#ifndef INC_STM_MCLOCK_H_
#define INC_STM_MCLOCK_H_

#include "main.h"

#define CLOCK_PERIOD_MS 1

typedef enum MCL_ClockState {
	MCL_STATE_BUSY = 0,
	MCL_STATE_TIMEOUT = 1,
	MCL_STATE_INVALID = 999
} MCL_ClockState;

typedef struct MCL_Clock {
	uint32_t count;
	uint32_t maxCount;

	MCL_ClockState state;

	uint32_t timeScale;
} MCL_Clock;


void MCL_Init(MCL_Clock* clock, uint32_t maxCount);
void MCL_CallBack_Timer(MCL_Clock* clock);
MCL_ClockState MCL_Pull(MCL_Clock* clock);

#endif /* INC_STM_MCLOCK_H_ */

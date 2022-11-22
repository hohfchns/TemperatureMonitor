#include <MClock/include/stm_mclock.h>


void MCL_Init(MCL_Clock* clock, uint32_t maxCount)
{
	clock->count = 0;
	clock->maxCount = maxCount;

	clock->state = MCL_STATE_BUSY;

	clock->timeScale = 1;
}

/* @brief ONLY CALL ON TIMEOUT OF 1MS */
void MCL_CallBack_Timer(MCL_Clock* clock)
{
	// Timer only increments when state is busy
	if (clock->state == MCL_STATE_BUSY)
	{
		clock->count += CLOCK_PERIOD_MS * clock->timeScale;
	}

	if (clock->count >= clock->maxCount)
	{
		// Waits for user to pull state
		clock->state = MCL_STATE_TIMEOUT;
	}
}

MCL_ClockState MCL_Pull(MCL_Clock* clock)
{
	MCL_ClockState pullState = clock->state;

	// Pull if timer is stopped at timeout state
	if (pullState == MCL_STATE_TIMEOUT)
	{
		clock->count = 0;
		clock->state = MCL_STATE_BUSY;
	}

	return pullState;
}




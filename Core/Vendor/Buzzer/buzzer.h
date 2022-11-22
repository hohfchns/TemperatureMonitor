#ifndef VENDOR_BUZZER_BUZZER_H_
#define VENDOR_BUZZER_BUZZER_H_

#include <stdio.h>
#include "main.h"
#include "stm_mclock.h"

class Buzzer {
public:
	Buzzer(uint32_t* seq, uint32_t* durs, TIM_HandleTypeDef* freqTim);
	void SetFrequency(uint32_t desired);
	int Update();
	void Continue();
	void Reset();

	void SetOn(int on);
	void Toggle();

	uint32_t GetDur();
	uint32_t GetDurp(size_t pos);
	uint32_t GetNote();
	uint32_t GetNotep(size_t pos);

private:
	uint32_t* _seq, *_durs;
	size_t _seqPos;
	int _isOn;

	TIM_HandleTypeDef *_freqTim;
	MCL_Clock _durClock;
} ;


#endif /* VENDOR_BUZZER_BUZZER_H_ */

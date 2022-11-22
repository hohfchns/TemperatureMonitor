#ifndef VENDOR_SIMPLEBUZZER_SIMPLE_BUZZER_H_
#define VENDOR_SIMPLEBUZZER_SIMPLE_BUZZER_H_

#include "main.h"

namespace SimpleBuzzer {

class Buzzer {
public:
	Buzzer(TIM_HandleTypeDef* htim, uint8_t timChannel);
	Buzzer();

	void On();
	void Off();
	bool Toggle();

private:
	TIM_HandleTypeDef* _htim;
	uint8_t _timChannel;

	bool _isOn;
};

}

#endif /* VENDOR_SIMPLEBUZZER_SIMPLE_BUZZER_H_ */

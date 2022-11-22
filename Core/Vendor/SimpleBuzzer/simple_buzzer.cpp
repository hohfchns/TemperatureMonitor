#include "simple_buzzer.h"

namespace SimpleBuzzer {

Buzzer::Buzzer(TIM_HandleTypeDef* htim, uint8_t timChannel):
_htim(htim), _timChannel(timChannel), _isOn(false)
{

}

Buzzer::Buzzer() {}

void Buzzer::On()
{
	HAL_TIM_Base_Start(_htim);
	HAL_TIM_PWM_Start(_htim, _timChannel);
	__HAL_TIM_SET_AUTORELOAD(_htim,150);

	_isOn = true;
}

void Buzzer::Off()
{
	HAL_TIM_Base_Stop(_htim);
	HAL_TIM_PWM_Stop(_htim, _timChannel);

	_isOn = false;
}

bool Buzzer::Toggle()
{
	if (_isOn)
	{
		Off();
	}
	else
	{
		On();
	}

	return _isOn;
}

}

#include "buzzer.h"
#include "buzzer_typedef.h"

using namespace BuzzerTypes;

Buzzer::Buzzer(uint32_t* seq, uint32_t* durs, TIM_HandleTypeDef* freqTim)
{
  _seq = seq;
  _durs = durs;
  _freqTim = freqTim;

  _seqPos = 0;
  _isOn = 0;

  MCL_Init(&_durClock, *durs);
//  HAL_TIM_Base_Stop(_freqTim);
//  HAL_TIM_PWM_Stop(_freqTim, TIM_CHANNEL_1);
//  __HAL_TIM_SET_COUNTER(_freqTim, 0);
}

int Buzzer::Update()
{
	if (_seqPos != 0)
	{
		return 0;
	}
//	MCL_CallBack_Timer(&_durClock);
//	if (MCL_Pull(&_durClock) != MCL_STATE_TIMEOUT)
//	{
//		return 0;
//	}

	if (!_isOn)
	{
		HAL_TIM_Base_Stop(_freqTim);
		HAL_TIM_PWM_Stop(_freqTim, TIM_CHANNEL_1);

		return 0;
	}
	else
	{
		HAL_TIM_Base_Start(_freqTim);
		HAL_TIM_PWM_Start(_freqTim, TIM_CHANNEL_1);
	}

	if (GetNote() == BZN_P)
	{
		HAL_TIM_Base_Stop(_freqTim);
		HAL_TIM_PWM_Stop(_freqTim, TIM_CHANNEL_1);
	}

	__HAL_TIM_SET_COUNTER(_freqTim, 0);
	SetFrequency(GetNote());

	_durClock.maxCount = GetDur();

	Continue();

	return 1;
}

void Buzzer::Continue()
{
	_seqPos++;

	if (GetNote() == BZN_EN || GetDur() == BZN_EN)
	{
		_seqPos = 0;
	}
}

void Buzzer::SetFrequency(uint32_t desired)
{
	uint32_t div1 = TIMER_CLOCK / TIMER_PRESCALER;
	uint32_t targetPeriod = div1 / desired;

	__HAL_TIM_SET_AUTORELOAD(_freqTim, targetPeriod);
	__HAL_TIM_SET_COMPARE(_freqTim, TIM_CHANNEL_1, targetPeriod / 2);
}

void Buzzer::Reset()
{
	_seqPos = 0;
}

void Buzzer::SetOn(int on)
{
	_isOn = on;
}

void Buzzer::Toggle()
{
	_isOn = !(_isOn);
}

uint32_t Buzzer::GetDur()
{
	return *(_durs + _seqPos);
}
uint32_t Buzzer::GetDurp(size_t pos)
{
	return *(_durs + pos);
}
uint32_t Buzzer::GetNote()
{
	return *(_seq + _seqPos);
}
uint32_t Buzzer::GetNotep(size_t pos)
{
	return *(_seq + pos);
}


#ifndef VENDOR_DHT11_DHT11_H_
#define VENDOR_DHT11_DHT11_H_

#include "main.h"

#define MAX_BITS 40

class DHT11 {
public:
	DHT11(GPIO_TypeDef * gpioPort, uint16_t gpioPin, TIM_HandleTypeDef* tim);
	void Update();
	void ReadAsync();
	double GetTemp() const;

	void OnGpioInterrupt(uint16_t pin);

	void _OverrideTemp(double temp);

private:
	bool HasData();

	void SetGpioOutput();
	void SetGpioInput();
	void SetGpioExti();

private:

	GPIO_TypeDef * _gpioPort;
	uint16_t _gpioPin;
	TIM_HandleTypeDef* _timer;
	int _counter;
	int _maxCounter;
	int _state;
	uint8_t _data[5];
	int _bit;
	double _tempLatest;
	double _temperature;
	double _humidity;
};

#endif /* VENDOR_DHT11_DHT11_H_ */

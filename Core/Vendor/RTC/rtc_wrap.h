#ifndef VENDOR_RTC_RTC_WRAP_H_
#define VENDOR_RTC_RTC_WRAP_H_

#include "main.h"
#include "rtc_typedef.h"

namespace RTCWrapper {

class RTCW {
public:
	RTCW(I2C_HandleTypeDef * hi2c, uint32_t devAddr);
	void Write(uint16_t memAddr, uint8_t * buffer, uint16_t size);
	void Read(uint16_t memAddr, uint8_t * buffer, uint16_t size);
	void Start();
	void Stop();
	int IsRunning();
	void GetTime(RTCTypes::DateTime * dateTime);
	uint32_t GetSeconds();
	void SetTime(RTCTypes::DateTime * dateTime);

private:
	I2C_HandleTypeDef * _hi2c;
	uint8_t _devAddr;
};

}

#endif /* VENDOR_RTC_RTC_WRAP_H_ */

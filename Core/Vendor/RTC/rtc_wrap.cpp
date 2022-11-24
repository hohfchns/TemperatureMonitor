#include "rtc_wrap.h"

using namespace RTCTypes;

namespace RTCWrapper {

const uint32_t DaysInYear    = 365;
const uint32_t SecondsInMin  = 60;
const uint32_t SecondsInHour = 3600;
const uint32_t SecondsInDay  = 86400;

static int bcdToInt(uint8_t bcd);
static uint8_t intToBcd(int value, int minVal, int maxVal);

static const int _daysUntilMonth[] = {
		0,
		31,
		59,
		90,
		120,
		151,
		181,
		212,
		243,
		273,
		304,
		334,
		365
};

RTCW::RTCW(I2C_HandleTypeDef * hi2c, uint32_t devAddr) :
	_hi2c(hi2c),
	_devAddr(devAddr)
{
}

void RTCW::Write(uint16_t memAddr, uint8_t * buffer, uint16_t size)
{
	HAL_I2C_Mem_Write(_hi2c, _devAddr, memAddr, 1, buffer, size, 0xFF);
}

void RTCW::Read(uint16_t memAddr, uint8_t * buffer, uint16_t size)
{
	HAL_I2C_Mem_Read(_hi2c, _devAddr, memAddr, 1, buffer, size, 0xFF);
}

void RTCW::Start()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	sec &= ~RTC_START_STOP;
	HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);

	sec = sec;
}

void RTCW::Stop()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	sec |= RTC_START_STOP;
	HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
}

int RTCW::IsRunning()
{
	uint8_t sec = 0;
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, &sec, 1, 0xFF);
	return (sec & RTC_START_STOP) == 0;
}

void RTCW::GetTime(DateTime * dateTime)
{
	uint8_t buffer[RTC_DATE_TIME_SIZE];
	HAL_I2C_Mem_Read(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF);

	// remove stop bit if set
	buffer[0] &= ~RTC_START_STOP;
	dateTime->sec = bcdToInt(buffer[0]);
	dateTime->min = bcdToInt(buffer[1]);
	dateTime->hours = bcdToInt(buffer[2]);
	dateTime->weekDay = buffer[3] & 0x07;
	dateTime->day = bcdToInt(buffer[4]);
	dateTime->month = bcdToInt(buffer[5]);
	dateTime->year = bcdToInt(buffer[6]);
}

uint32_t RTCW::GetSeconds()
{
	// calculate seconds from 00:00:00 1/1/2020
	DateTime dateTime;
	GetTime(&dateTime);

	uint32_t seconds = dateTime.sec +
			dateTime.min * SecondsInMin +
			dateTime.hours * SecondsInHour +
			dateTime.day * SecondsInDay +
			_daysUntilMonth[dateTime.month - 1] * SecondsInDay +
			dateTime.year * DaysInYear * SecondsInDay;
	if (dateTime.year % 4 == 0 && dateTime.month > 2) {
		// if current year is a leap year and month is after February
		// add seconds for February 29
		seconds += SecondsInDay;
	}
	// add seconds for all previous leap years
	seconds += (dateTime.year / 4) * SecondsInDay;
	return seconds;
}

void RTCW::SetTime(DateTime * dateTime)
{
	uint8_t buffer[RTC_DATE_TIME_SIZE];

	buffer[0] = intToBcd(dateTime->sec, 0, 59);
	buffer[1] = intToBcd(dateTime->min, 0, 59);
	buffer[2] = intToBcd(dateTime->hours, 0, 59);
	buffer[3] = dateTime->weekDay < 1 || dateTime->weekDay > 7 ? 0 : dateTime->weekDay;
	buffer[4] = intToBcd(dateTime->day, 1, 31);
	buffer[5] = intToBcd(dateTime->month, 1, 12);
	buffer[6] = intToBcd(dateTime->year, 1, 99);

	HAL_I2C_Mem_Write(_hi2c, _devAddr, 0, 1, buffer, RTC_DATE_TIME_SIZE, 0xFF);
}

static int bcdToInt(uint8_t bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t intToBcd(int value, int minVal, int maxVal)
{
	if (value < minVal || value > maxVal) {
		return 0;
	}

	return ((value / 10) << 4) | (value % 10);
}

}

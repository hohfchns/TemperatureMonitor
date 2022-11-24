#ifndef VENDOR_RTC_RTC_TYPEDEF_H_
#define VENDOR_RTC_RTC_TYPEDEF_H_

#define RTC_START_STOP      (1 << 7)
#define RTC_DATE_TIME_SIZE  7
#define DEVICE_ADDR         0xD0

namespace RTCTypes {
typedef struct DateTime_
{
	int sec;
	int min;
	int hours;
	int weekDay;
	int day;
	int month;
	int year;
} DateTime;
}

#endif /* VENDOR_RTC_RTC_TYPEDEF_H_ */

#include "command_defs.h"
#include <stdio.h>
#include <string.h>
#include "simple_buzzer.h"
#include "dht11.h"
#include "rtc_wrap.h"
#include "global_params.h"

using namespace SimpleBuzzer;

namespace AppCommands
{

BuzzerOn g_buzzerOn;
BuzzerOff g_buzzerOff;
DHTRead g_dhtRead;
DebugDHTOverride g_dhtOverride;
RTCSetTime g_setRTC;
RTCGetTime g_getRTC;

void InitCommands(SimpleBuzzer::Buzzer* buzzer, DHT11* dht, RTCWrapper::RTCW* rtc)
{
	g_buzzerOn = BuzzerOn(buzzer);
	g_buzzerOff = BuzzerOff(buzzer);
	g_dhtRead = DHTRead(dht);
	g_dhtOverride = DebugDHTOverride(dht);
	g_setRTC = RTCSetTime(rtc);
	g_getRTC = RTCGetTime(rtc);

	CMD::RegisterCommand("buzzer-on", &g_buzzerOn);
	CMD::RegisterCommand("buzzer-off", &g_buzzerOff);
	CMD::RegisterCommand("get-temp", &g_dhtRead);
	CMD::RegisterCommand("d_over", &g_dhtOverride);
	CMD::RegisterCommand("set-rtc", &g_setRTC);
	CMD::RegisterCommand("get-rtc", &g_getRTC);
}

bool HandleCommand(const char* cmd)
{
	return CMD::SendCmd(cmd);
}

BuzzerOn::BuzzerOn() {}

BuzzerOn::BuzzerOn(Buzzer* buzzer) :
_buzzer(buzzer)
{}

void BuzzerOn::Execute(const char* argv)
{
	_buzzer->On();

	printf("Set buzzer to on\r\n");
}

BuzzerOff::BuzzerOff() {}

BuzzerOff::BuzzerOff(Buzzer* buzzer) :
_buzzer(buzzer)
{}

void BuzzerOff::Execute(const char* argv)
{
	_buzzer->Off();

	printf("Set buzzer to off\r\n");
}

DHTRead::DHTRead() {}

DHTRead::DHTRead(const DHT11* dht) :
_dht(dht)
{

}

void DHTRead::Execute(const char* argv)
{
	printf("Dht temperature is: %d\r\n", (uint8_t)_dht->GetTemp());
}

DebugDHTOverride::DebugDHTOverride() {}

DebugDHTOverride::DebugDHTOverride(DHT11* dht) :
_dht(dht)
{

}

void DebugDHTOverride::Execute(const char* argv)
{
	char cmdName[1];
	unsigned int newTemp;
	sscanf(argv, "%s %d", cmdName, &newTemp);

	_dht->_OverrideTemp((double)newTemp);
	printf("Dht temperature set to : %d\r\n", (unsigned int)_dht->GetTemp());
}

RTCSetTime::RTCSetTime(RTCWrapper::RTCW* rtc) :
		_rtc(rtc)
{}

RTCSetTime::RTCSetTime()
{}

void RTCSetTime::Execute(const char* argv)
{
	RTCTypes::DateTime dateTime;
	_rtc->GetTime(&dateTime);

	int* arr[] = {
			&dateTime.year, &dateTime.month, &dateTime.day, &dateTime.hours, &dateTime.min, &dateTime.sec
	};

	char buffer[30];
	strcpy(buffer, argv);
	char* token = strtok(buffer, " ");

	int argc = 0;
	while (token != NULL)
	{
		if (argc > 0) {
			sscanf(token, "%d", arr[argc - 1]);
		}

		token = strtok(NULL, ":");
		argc++;
	}

	if (argc <= 1)
	{
		printf("Please provide argument in a format seperation with ':'\r\n");
		return;
	}

	_rtc->SetTime(&dateTime);
}

RTCGetTime::RTCGetTime(RTCWrapper::RTCW* rtc) :
		_rtc(rtc)
{}

RTCGetTime::RTCGetTime()
{}

void RTCGetTime::Execute(const char* argv)
{
	RTCTypes::DateTime dateTime;
	_rtc->GetTime(&dateTime);
	printf("time is %d:%d:%d:%d:%d:%d\r\n", dateTime.year, dateTime.month, dateTime.day, dateTime.hours, dateTime.min, dateTime.sec);
}

}


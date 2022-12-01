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

namespace Argument {
enum FlagType {
	SHORT,
	FULL,
	NONE
};

}

static bool IsCom(const char* c, const char* c2)
{
	return strcmp(c, c2) == 0;
}

static BuzzerOn g_buzzerOn;
static BuzzerOff g_buzzerOff;
static DHTRead g_dhtRead;
static DebugDHTOverride g_dhtOverride;
static RTCSetTime g_setRTC;
static RTCGetTime g_getRTC;

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
	CMD::RegisterCommand("set-rtc", &g_setRTC);
	CMD::RegisterCommand("get-rtc", &g_getRTC);
#ifdef APP_DEBUG
	CMD::RegisterCommand("d_over", &g_dhtOverride);
#endif
}

bool HandleCommand(const char* cmd)
{
	return CMD::SendCmd(cmd);
}

static char* getArg(char* command, char* out_Param, char* out_Arg, Argument::FlagType& out_ArgType)
{
	static char* token = 0;
	if (command != NULL){
		 token = strtok(command, " ");
		 token = strtok(NULL, "-");
	}
	else
	{
		 token = strtok(NULL, "-");
	}

	if (token != NULL)
	{
		out_ArgType = Argument::SHORT;

		if (*(token - 2) == '-')
		{
			out_ArgType = Argument::FULL;
		}

		sscanf(token, "%s %s", out_Arg, out_Param);
		return out_Param;
	}

	return NULL;

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

static void formatTime(const char* formatted, RTCTypes::DateTime* out_Time)
{
	char buffer[30];
	strcpy(buffer, formatted);

	int* arr[] = {
			&out_Time->year, &out_Time->month, &out_Time->day, &out_Time->hours, &out_Time->min, &out_Time->sec
	};

	char* token = strtok(buffer, ":");
	int i = 0;
	while (token != NULL)
	{
		if (i >= 6) {
			break;
		}

		sscanf(token, "%d", arr[i]);
		token = strtok(NULL, ":");
		i++;
	}
}

RTCSetTime::RTCSetTime(RTCWrapper::RTCW* rtc) :
		_rtc(rtc)
{}

RTCSetTime::RTCSetTime()
{}

void RTCSetTime::Execute(const char* argv)
{
	RTCTypes::DateTime time;
	_rtc->GetTime(&time);

	char buffer[30];
	strcpy(buffer, argv);

	char param[30];
	char flag[30];
	char* p = param;

	Argument::FlagType type;
	p = getArg(buffer, p, flag, type);

	while (p != NULL)
	{
		if (type == Argument::SHORT)
		{
			switch (*flag) {
			case 'y':
				;
				sscanf(param, "%d", &time.year);
				break;
			case 'm':
				;
				sscanf(param, "%d", &time.month);
				break;
			case 'd':
				;
				sscanf(param, "%d", &time.day);
				break;
			case 'H':
				;
				sscanf(param, "%d", &time.hours);
				break;
			case 'M':
				;
				sscanf(param, "%d", &time.min);
				break;
			case 'S':
				;
				sscanf(param, "%d", &time.sec);
				break;
			case 'f':
				;
				formatTime(param, &time);
				break;
			default:
				break;
			}
		}
		else if (type == Argument::FULL)
		{
			if (IsCom(flag, "year"))
			{
				sscanf(param, "%d", &time.year);
			}
			else if (IsCom(flag, "month"))
			{
				sscanf(param, "%d", &time.day);
			}
			else if (IsCom(flag, "day"))
			{
				sscanf(param, "%d", &time.day);
			}
			if (IsCom(flag, "hour"))
			{
				sscanf(param, "%d", &time.hours);
			}
			else if (IsCom(flag, "minute"))
			{
				sscanf(param, "%d", &time.min);
			}
			else if (IsCom(flag, "second"))
			{
				sscanf(param, "%d", &time.sec);
			}
			else if (IsCom(flag, "format"))
			{
				formatTime(param, &time);
			}
		}

		p = getArg(NULL, param, flag, type);
	}

	_rtc->SetTime(&time);
}

RTCGetTime::RTCGetTime(RTCWrapper::RTCW* rtc) :
		_rtc(rtc)
{}

RTCGetTime::RTCGetTime()
{}

void RTCGetTime::Execute(const char* argv)
{
	RTCTypes::DateTime out_Time;
	_rtc->GetTime(&out_Time);
	printf("time is %d:%d:%d:%d:%d:%d\r\n", out_Time.year, out_Time.month, out_Time.day, out_Time.hours, out_Time.min, out_Time.sec);
}

}


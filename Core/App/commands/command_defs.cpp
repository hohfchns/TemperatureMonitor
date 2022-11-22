#include "command_defs.h"
#include <stdio.h>
#include "simple_buzzer.h"
#include "dht11.h"

using namespace SimpleBuzzer;

namespace AppCommands
{

BuzzerOn g_buzzerOn;
BuzzerOff g_buzzerOff;
DHTRead g_dhtRead;

void InitCommands(SimpleBuzzer::Buzzer* buzzer, DHT11* dht, double* dhtBufTemp)
{
	g_buzzerOn = BuzzerOn(buzzer);
	g_buzzerOff = BuzzerOff(buzzer);
	g_dhtRead = DHTRead(dht, dhtBufTemp);

	CMD::RegisterCommand("buzzer-on", &g_buzzerOn);
	CMD::RegisterCommand("buzzer-off", &g_buzzerOff);
	CMD::RegisterCommand("get-temp", &g_dhtRead);
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

DHTRead::DHTRead(const DHT11* dht, double* bufTemp) :
_dht(dht), _bufTemp(bufTemp)
{

}

void DHTRead::Execute(const char* argv)
{
	*_bufTemp = _dht->GetTemp();

	printf("Dht temperature is: %u\r\n", (uint16_t)*_bufTemp);
}

}

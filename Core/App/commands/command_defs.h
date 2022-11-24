#ifndef APP_COMMANDS_COMMAND_DEFS_H_
#define APP_COMMANDS_COMMAND_DEFS_H_

#include "cli_command.h"

namespace SimpleBuzzer {
class Buzzer;
}
namespace RTCWrapper {
class RTCW;
}

class DHT11;

namespace AppCommands {

void InitCommands(SimpleBuzzer::Buzzer* buzzer, DHT11* dht, RTCWrapper::RTCW* rtc);
bool HandleCommand(const char* cmd);

class BuzzerOn : public CMD::ACommand {
public:
	BuzzerOn(SimpleBuzzer::Buzzer* buzzer);
	BuzzerOn();

	void Execute(const char* argv) override;

private:
	SimpleBuzzer::Buzzer* _buzzer;
};

class BuzzerOff : public CMD::ACommand {
public:
	BuzzerOff(SimpleBuzzer::Buzzer* buzzer);
	BuzzerOff();

	void Execute(const char* argv) override;

private:
	SimpleBuzzer::Buzzer* _buzzer;
};

class DHTRead : public CMD::ACommand {
public:
	DHTRead();
	DHTRead(const DHT11* dht);

	void Execute(const char* argv) override;

private:
	const DHT11* _dht;
	double* _bufTemp;
};

class DebugDHTOverride : public CMD::ACommand {
public:
	DebugDHTOverride();
	DebugDHTOverride(DHT11* dht);

	void Execute(const char* argv) override;

private:
	DHT11* _dht;
	double* _bufTemp;
};


class RTCSetTime : public CMD::ACommand {
public:
	RTCSetTime(RTCWrapper::RTCW* rtc);
	RTCSetTime();

	void Execute(const char* argv) override;

private:
	RTCWrapper::RTCW* _rtc;
};

class RTCGetTime : public CMD::ACommand {
public:
	RTCGetTime(RTCWrapper::RTCW* rtc);
	RTCGetTime();

	void Execute(const char* argv) override;

private:
	RTCWrapper::RTCW* _rtc;
};

}

#endif /* APP_COMMANDS_COMMAND_DEFS_H_ */

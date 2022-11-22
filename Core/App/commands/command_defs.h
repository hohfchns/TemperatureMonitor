#ifndef APP_COMMANDS_COMMAND_DEFS_H_
#define APP_COMMANDS_COMMAND_DEFS_H_

#include "cli_command.h"

namespace SimpleBuzzer {
class Buzzer;
}

class DHT11;

namespace AppCommands {

void InitCommands(SimpleBuzzer::Buzzer* buzzer, DHT11* dht, double* dhtBufTemp);
bool HandleCommand(const char* cmd);

class BuzzerOn : public CMD::ACommand {
public:
	BuzzerOn(SimpleBuzzer::Buzzer* buzzer);
	BuzzerOn();

	void Execute(const char* argv) override;

	SimpleBuzzer::Buzzer* _buzzer;
};

class BuzzerOff : public CMD::ACommand {
public:
	BuzzerOff(SimpleBuzzer::Buzzer* buzzer);
	BuzzerOff();

	void Execute(const char* argv) override;

	SimpleBuzzer::Buzzer* _buzzer;
};

class DHTRead : public CMD::ACommand {
public:
	DHTRead();
	DHTRead(const DHT11* dht, double* bufTemp);

	void Execute(const char* argv) override;

private:
	const DHT11* _dht;
	double* _bufTemp;
};

}

#endif /* APP_COMMANDS_COMMAND_DEFS_H_ */

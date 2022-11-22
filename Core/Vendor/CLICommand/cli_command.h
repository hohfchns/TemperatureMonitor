#ifndef VENDOR_CLICOMMAND_CLI_COMMAND_H_
#define VENDOR_CLICOMMAND_CLI_COMMAND_H_

namespace CMD {
class ACommand {
public:
	virtual void Execute(const char* argv) = 0;
};

bool RegisterCommand(const char* cmdName, ACommand* cmd);
bool RemoveCommand(const char* cmdName);

bool SendCmd(const char* cmd);
}

#endif /* VENDOR_CLICOMMAND_CLI_COMMAND_H_ */

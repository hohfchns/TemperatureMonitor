#include "cli_command.h"
#include "cli_typedef.h"
#include <stdio.h>
#include <string.h>

namespace CMD {

struct RegisteredCommand {
public:
	const char* name;
	ACommand* cmd;
};

static RegisteredCommand g_cmds[CLI_MAX_COMMANDS];
static size_t g_numCmds = 0;

static RegisteredCommand* Find(const char* cmdName);
static RegisteredCommand* Find(const char* cmdName, size_t& outIndex);

bool RegisterCommand(const char* cmdName, ACommand* cmd)
{
	if (g_numCmds >= CLI_MAX_COMMANDS)
	{
		return false;
	}

	g_cmds[g_numCmds++] = { cmdName, cmd };
	return true;
}

bool RemoveCommand(const char* cmdName)
{
	size_t i = 0;
	RegisteredCommand* registeredCmd = Find(cmdName, i);
	if (registeredCmd)
	{
		g_cmds[i] = g_cmds[--g_numCmds];
		return true;
	}

	return false;
}

bool SendCmd(const char* cmd)
{
	char name[CLI_MAX_NAME_SIZE];
	sscanf(cmd, "%s", name);

	RegisteredCommand* registeredCmd = Find(name);
	if (registeredCmd)
	{
		registeredCmd->cmd->Execute(cmd);
		return true;
	}

	return false;
}

static RegisteredCommand* Find(const char* cmdName)
{
	for (size_t i = 0; i < g_numCmds; i++)
	{
		if (strcmp(g_cmds[i].name, cmdName) == 0)
		{
			return &g_cmds[i];
		}
	}

	return nullptr;
}

static RegisteredCommand* Find(const char* cmdName, size_t& outIndex)
{
	for (size_t i = 0; i < g_numCmds; i++)
	{
		if (strcmp(g_cmds[i].name, cmdName) == 0)
		{
			outIndex = i;
			return &g_cmds[i];
		}
	}

	return nullptr;
}
}

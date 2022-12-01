#include "fat_log.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define MAX_MESSAGE_LEN 512

void _FAT_LOG(const char* sourceFilepath, int sourceLine, FatBus::ID id, const char* content)
{
	char format[MAX_MESSAGE_LEN];
	sprintf(format,
			"-----------------\r\n"
			"LOG:DEBUG\r\n"
		    "FILE:\r\n"
		    "%s\r\n"
		    "LINE:\r\n"
		    "%d\r\n"
		    "MESSAGE:\r\n"
		    "%s\r\n"
			"-----------------\r\n",
			sourceFilepath, sourceLine, content);

	FatBus::ErrorCode err;
	int written = 0;

	FatBus::FBus& bus = FatBus::FBus::Get();
	bus.FWrite(format, id, err, written);
	if (err != FR_OK)
	{
		printf("");
		return;
	}

//	printf(format);
}

void _FAT_LOGF(const char* sourceFilepath, int sourceLine, FatBus::ID id, const char* format, ...)
{
	char message[MAX_MESSAGE_LEN - 80];
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(message, MAX_MESSAGE_LEN - 81, format, argptr);
//	vsprintf(message, format, argptr);
	va_end(argptr);

	char out[MAX_MESSAGE_LEN];
	sprintf(out,
			"-----------------\r\n"
			"LOG:DEBUG\r\n"
		    "FILE:\r\n"
		    "%s\r\n"
		    "LINE:\r\n"
		    "%d\r\n"
		    "MESSAGE:\r\n"
		    "%s\r\n"
			"-----------------\r\n",
			sourceFilepath, sourceLine, message);

	FatBus::ErrorCode err;
	int written = 0;

	FatBus::FBus& bus = FatBus::FBus::Get();
	bus.FWrite(out, id, err, written);
	if (err != FR_OK)
	{
		printf("");
		return;
	}
}


#ifndef VENDOR_FATLOG_FAT_LOG_H_
#define VENDOR_FATLOG_FAT_LOG_H_

#include "ff.h"
#include "fat_bus.h"

#define FAT_LOG(bus, id, content) _FAT_LOG(__FILE__, __LINE__, id, content)
#define FAT_LOGF(bus, id, format, ...) _FAT_LOGF(__FILE__, __LINE__, id, format, __VA_ARGS__)

void _FAT_LOG(const char* sourceFilepath, int sourceLine, FatBus::ID id, const char* content);
void _FAT_LOGF(const char* sourceFilepath, int sourceLine, FatBus::ID id, const char* format, ...);


#endif /* VENDOR_FATLOG_FAT_LOG_H_ */

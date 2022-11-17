#ifndef APP_APP_H_
#define APP_APP_H_

#include "main.h"
#include "cmsis_os.h"

extern "C" {
int _write(int fd, char* ptr, int len);
void APP_Main();
void cliTask(void* argument);
void mainTask(void* argument);
void ledTask(void* argument);
void updateTask(void* argument);
}

#endif /* APP_APP_H_ */

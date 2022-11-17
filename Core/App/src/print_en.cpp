#include "print_en.h"
#include "main.h"
#include <stdio.h>
#include "global_types.h"

extern UART_HandleTypeDef huart2;
int _write(int fd, char* ptr, int len) {
	HAL_UART_Transmit(&huart2, (uint8_t *) ptr, len, HAL_MAX_DELAY);

    return len;
}

/*
 * commTaskFuncs.c
 *
 *  Created on: Oct 20, 2022
 *      Author: student
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "command_line.h"

extern UART_HandleTypeDef huart2;

static uint8_t cmdbuffer[MAX_BUFFER_LENGTH];
static int cmdcount = 0;
static int cmdprint = 0;

namespace CommandLine{
int Get(const char** out_pdata)
{
	uint8_t ch;

	HAL_StatusTypeDef Status = HAL_UART_Receive(&huart2, &ch, 1, 10);
	if (Status != HAL_OK)
	{
		if ((huart2.Instance->ISR & USART_ISR_ORE) != 0)
		{
			__HAL_UART_CLEAR_OREFLAG(&huart2);
		}

		// here we have a time to print the command
		while (cmdprint < cmdcount)
		{
			HAL_UART_Transmit(&huart2, &cmdbuffer[cmdprint++], 1, 0xFF);
//			printf("%c", (char)cmdbuffer[cmdprint++]);
		}

		return 0;
	}

	if (ch == '\b')
	{
//		printf("\b \b");
		HAL_UART_Transmit(&huart2, (uint8_t*) "\b \b", 3, 0xFF);
		cmdcount--;
		cmdprint--;
		return 0;
	}

	if (ch != '\r' && ch != '\n')
	{
		//HAL_UART_Transmit(&huart2, &ch, 1, 0xFFFF);

		if (cmdcount >= MAX_BUFFER_LENGTH)
		{
			cmdcount = 0;
			cmdprint = 0;
		}

		cmdbuffer[cmdcount++] = ch;
		return 0;
	}

	// here we have a time to print the command
	while (cmdprint < cmdcount)
	{
		HAL_UART_Transmit(&huart2, &cmdbuffer[cmdprint++], 1, 0xFF);
//		printf("%c", (char)cmdbuffer[cmdprint++]);
	}

	HAL_UART_Transmit(&huart2, (uint8_t*) "\r\n", 2, 0xFF);
//	printf("\r\n");

	cmdbuffer[cmdcount] = 0;
	cmdcount = 0;
	cmdprint = 0;

	*out_pdata = (const char*)cmdbuffer;
	return 1;
}

}



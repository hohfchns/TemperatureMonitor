#include "app.h"
#include "command_line.h"
#include "print_en.h"
#include "dht11.h"
#include "global_params.h"
#include "global_types.h"

using namespace ThresholdParams;
using namespace TempTypes;

DHT11 g_dhtSensor = DHT11();

TempState g_thresholdState = TEMP_STATE_OK;

void mainTask(void* argument)
{
	for (;;)
	{
		osDelay(1);
	}
}

void updateTask(void* argument)
{
	for (;;)
	{
		int temp = g_dhtSensor.Update();

		if (temp > THRESHOLD_CRITICAL)
		{
			g_thresholdState = TEMP_STATE_CRITICAL;
		}
		else if (temp > THRESHOLD_WARNING)
		{
			g_thresholdState = TEMP_STATE_WARNING;
		}
		else
		{
			g_thresholdState = TEMP_STATE_OK;
		}

		osDelay(1000);
	}
}

void ledTask(void* argument)
{
	for (;;)
	{
		if (g_thresholdState == TEMP_STATE_OK)
		{
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		}

		osDelay(500);
	}
}

void cliTask(void* argument)
{
	printf("\r\n");

	for (;;)
	{
		const char* cmd = 0;
		bool gotCommand = CommandLine::Get(&cmd);
		if (gotCommand)
		{
			printf("Got the command '%s'\r\n", cmd);
		}

		osDelay(1);
	}
}

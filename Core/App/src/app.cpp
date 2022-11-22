#include "app.h"
#include "command_line.h"
#include "print_en.h"
#include "dht11.h"
#include "global_params.h"
#include "global_types.h"
#include "command_defs.h"
#include "simple_buzzer.h"

using namespace ThresholdParams;
using namespace TempTypes;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
const HALWrap::Timer g_tim3 = { &htim3, TIM_CHANNEL_1 };

DHT11 g_dhtSensor = DHT11(DHT11_GPIO_Port, DHT11_Pin, &htim1);
double g_temp = 0;

TempState g_thresholdState = TEMP_STATE_OK;

SimpleBuzzer::Buzzer g_buzzer = SimpleBuzzer::Buzzer(g_tim3.htim, g_tim3.channel);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	g_dhtSensor.OnGpioInterrupt(GPIO_Pin);
}

void mainTask(void* argument)
{
	for (;;)
	{
		double temp = g_dhtSensor.GetTemp();
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
	}
}

void buzzerTask(void *argument)
{
	for(;;)
	{
//	g_buzzer.SetOn(1);
//	g_buzzer.Update();


		osDelay(1);
	}
}

void updateTask(void* argument)
{
	for (;;)
	{
		g_dhtSensor.ReadAsync();

		osDelay(1000);

		g_dhtSensor.Update();
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

	AppCommands::InitCommands(&g_buzzer, &g_dhtSensor, &g_temp);
	for (;;)
	{
		const char* cmd = 0;
		bool gotCommand = CommandLine::Get(&cmd);
		if (gotCommand)
		{
			if (!AppCommands::HandleCommand(cmd))
			{
				printf("Invalid command '%s'\r\n", cmd);
			}
		}

		osDelay(1);
	}
}

#include "app.h"
#include "command_line.h"
#include "print_en.h"
#include "dht11.h"
#include "global_params.h"
#include "global_types.h"
#include "command_defs.h"
#include "simple_buzzer.h"
#include "rtc_wrap.h"

#define RTC_MAGIC_WORD 0x5ABU
#define RTC_MAGIC_WORD_ADDR 0x10
#define RTC_MAGIC_WORD_SIZE 2

using namespace ThresholdParams;
using namespace TempTypes;
using namespace RTCWrapper;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern I2C_HandleTypeDef hi2c1;

const HALWrap::Timer g_tim3 = { &htim3, TIM_CHANNEL_1 };

DHT11 g_dhtSensor = DHT11(DHT11_GPIO_Port, DHT11_Pin, &htim1);
double g_temp = 0;
RTCW g_rtc = RTCW(&hi2c1, 0xD0);

TempState g_thresholdState = TEMP_STATE_OK;

SimpleBuzzer::Buzzer g_buzzer = SimpleBuzzer::Buzzer(g_tim3.htim, g_tim3.channel);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	g_dhtSensor.OnGpioInterrupt(GPIO_Pin);
}

static void initializeRTC()
{
	uint16_t buffer = 0;
	g_rtc.Read(RTC_MAGIC_WORD_ADDR, (uint8_t*)&buffer, RTC_MAGIC_WORD_SIZE);
	if (buffer != RTC_MAGIC_WORD)
	{
		uint16_t magicWord = RTC_MAGIC_WORD;
		g_rtc.Start();
		RTCTypes::DateTime startTime = {0, 0, 0, 0, 0, 0, 0};
		g_rtc.SetTime(&startTime);
		g_rtc.Write(RTC_MAGIC_WORD_ADDR, (uint8_t*)&magicWord, RTC_MAGIC_WORD_SIZE);

		printf("First startup detected! Please initialize RTC with the set-rtc command!\r\n");
	}
}

void mainTask(void* argument)
{
	initializeRTC();
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

void updateTask(void* argument)
{
	for (;;)
	{
		g_dhtSensor.ReadAsync();

		osDelay(1000);

		g_dhtSensor.Update();
	}
}

void buzzerTask(void *argument)
{
	for(;;)
	{
		switch (g_thresholdState)
		{
		case TEMP_STATE_CRITICAL:
			;
			g_buzzer.On();
			break;
		default:
			;
			g_buzzer.Off();
			break;
		}

		osDelay(1);
	}
}

void ledTask(void* argument)
{
	for (;;)
	{
		switch (g_thresholdState)
		{
		case TEMP_STATE_OK:
			;
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
			break;
		default:
			;
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			break;
		}

		osDelay(500);
	}
}

void cliTask(void* argument)
{
	printf("\r\n");

	AppCommands::InitCommands(&g_buzzer, &g_dhtSensor, &g_rtc);
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

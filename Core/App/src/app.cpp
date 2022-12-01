#include "app.h"
#include "command_line.h"
#include "print_en.h"
#include "dht11.h"
#include "global_params.h"
#include "global_types.h"
#include "command_defs.h"
#include "simple_buzzer.h"
#include "rtc_wrap.h"
#include "rtc_typedef.h"
#include "fat_bus.h"
#include "fat_log.h"

using namespace ThresholdParams;
using namespace TempTypes;
using namespace RTCWrapper;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern I2C_HandleTypeDef hi2c1;

static const HALWrapper::Timer g_tim3 = { &htim3, TIM_CHANNEL_1 };
static const HALWrapper::Timer g_DhtTimer = { &htim1 };
static const HALWrapper::Gpio g_DhtGpio = { DHT11_GPIO_Port, DHT11_Pin };
static const HALWrapper::Gpio g_LedGpio = { LD2_GPIO_Port, LD2_Pin };

static DHT11 g_dhtSensor = DHT11(g_DhtGpio.gpiox, g_DhtGpio.pin, g_DhtTimer.htim);
static double g_temp = 0;
static RTCW g_rtc = RTCW(&hi2c1, 0xD0);

TempState g_thresholdState = TEMP_STATE_OK;

SimpleBuzzer::Buzzer g_buzzer = SimpleBuzzer::Buzzer(g_tim3.htim, g_tim3.channel);

FatBus::ID g_tempLogId;

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
		g_temp = g_dhtSensor.GetTemp();
		TempState last = g_thresholdState;

		if (g_temp > THRESHOLD_CRITICAL)
		{
			g_thresholdState = TEMP_STATE_CRITICAL;
		}
		else if (g_temp > THRESHOLD_WARNING)
		{
			g_thresholdState = TEMP_STATE_WARNING;
		}
		else
		{
			g_thresholdState = TEMP_STATE_OK;
		}

		if (g_temp > 0 && g_thresholdState != last)
		{
			FatBus::FBus& bus = FatBus::FBus::Get();
			RTCTypes::DateTime dateTime;
			g_rtc.GetTime(&dateTime);

			if (g_thresholdState == TEMP_STATE_CRITICAL)
			{
				FAT_LOGF(bus, g_tempLogId,
					"%d:%d:%d:%d:%d:%d\r\n"
					"THRESHOLD REACHED CRITICAL LEVEL"
					"TEMPERATURE: %d",
					dateTime.year, dateTime.month, dateTime.day, dateTime.hours, dateTime.min, dateTime.sec,
					(int)g_temp
					);

				bus.FFlush(g_tempLogId);
			}
			else if (g_thresholdState == TEMP_STATE_WARNING)
			{
				FAT_LOGF(bus, g_tempLogId,
					"%d:%d:%d:%d:%d:%d\r\n"
					"THRESHOLD REACHED WARNING LEVEL"
					"TEMPERATURE: %d",
					dateTime.year, dateTime.month, dateTime.day, dateTime.hours, dateTime.min, dateTime.sec,
					(int)g_temp
					);

				bus.FFlush(g_tempLogId);
			}
			else
			{
				FAT_LOGF(bus, g_tempLogId,
					"%d:%d:%d:%d:%d:%d\r\n"
					"THRESHOLD REACHED NORMAL LEVEL"
					"TEMPERATURE: %d",
					dateTime.year, dateTime.month, dateTime.day, dateTime.hours, dateTime.min, dateTime.sec,
					(int)g_temp
					);

				bus.FFlush(g_tempLogId);
			}
		}

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
			HAL_GPIO_WritePin(g_LedGpio.gpiox, g_LedGpio.pin, GPIO_PIN_RESET);
			break;
		default:
			;
			HAL_GPIO_TogglePin(g_LedGpio.gpiox, g_LedGpio.pin);
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

void flagTask(void* argument)
{
	for (;;)
	{
		osDelay(6000);
	}
}

void osTask(void* argument)
{
	osDelay(500);
	FatBus::FBus& bus = FatBus::FBus::Get();
	FatBus::ErrorCode err = FR_OK;
	FatBus::BusSuccess success = bus.Init(err);

	success = bus.FOpen("temp.log", FA_OPEN_APPEND | FA_WRITE, g_tempLogId, err);

	if (!success)
	{
		printf("Failed to initialize filesystem!\r\n");
	}

	for (;;)
	{
		RTCTypes::DateTime dateTime;
		g_rtc.GetTime(&dateTime);

		FAT_LOGF(bus, g_tempLogId,
				"%d:%d:%d:%d:%d:%d\r\n"
				"TEMPERATURE: %d",
				dateTime.year, dateTime.month, dateTime.day, dateTime.hours, dateTime.min, dateTime.sec,
				(int)g_temp
				);

		bus.FFlush(g_tempLogId);

		osDelay(60000);
	}

	bus.CloseAll();
}

#ifndef APP_INCLUDE_GLOBAL_TYPES_H_
#define APP_INCLUDE_GLOBAL_TYPES_H_

#include "main.h"

namespace CliGlobal {
enum CliContext {
	CONTEXT_0 = 0,
	CONTEXT_1 = 1,
	CONTEXT_2 = 2,
	CONTEXT_3 = 3,
	CONTEXT_4 = 4,
	CONTEXT_5 = 5,
};
}

namespace TempTypes {
enum TempState {
	TEMP_STATE_OK = 0,
	TEMP_STATE_WARNING,
	TEMP_STATE_CRITICAL
};
}

namespace HALWrapper {
struct Gpio {
	GPIO_TypeDef* gpiox;
	uint16_t pin;
};

struct Timer {
	TIM_HandleTypeDef* htim;
	uint8_t channel;
};

}
#endif /* APP_INCLUDE_GLOBAL_TYPES_H_ */

#ifndef APP_INCLUDE_PRINT_EN_H_
#define APP_INCLUDE_PRINT_EN_H_

#include <stdio.h>
#include "global_types.h"

extern "C" {
int _write(int fd, char* ptr, int len);
}

namespace Print {
	void requestContext(CliGlobal::CliContext context);
	void enableContext(CliGlobal::CliContext context);
}

#endif /* APP_INCLUDE_PRINT_EN_H_ */

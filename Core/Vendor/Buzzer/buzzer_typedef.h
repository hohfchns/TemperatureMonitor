#ifndef VENDOR_BUZZER_BUZZER_TYPEDEF_H_
#define VENDOR_BUZZER_BUZZER_TYPEDEF_H_

#define TIMER_CLOCK 80000000
#define TIMER_PRESCALER 799

namespace BuzzerTypes {

typedef enum Note {
	BZN_EN = 0,
	BZN_P = 1,
	BZN_C4 = 262,
	BZN_D4 = 294,
	BZN_E4 = 330,
	BZN_F4 = 370,
	BZN_G4 = 392,

	BZN_C5 = 523,
	BZN_D5 = 587,
	BZN_E5 = 659,
	BZN_F5 = 698,
	BZN_G5 = 784
} Note;


}

#endif /* VENDOR_BUZZER_BUZZER_TYPEDEF_H_ */

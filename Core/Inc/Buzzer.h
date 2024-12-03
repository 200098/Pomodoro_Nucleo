/*
 * Buzzer.h
 *
 *      Author: Szymon Frydrych
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

/*
 * Definitions
 */
#define PRESSED_TIME 50
#define CHANGE_STEP_TIME 500
#define FINISH_TIME 1500

/*
 * Struct of buzzer
 */
typedef struct
{
	GPIO_TypeDef* 	GpioPort;
	uint32_t 		GpioPin;
	uint32_t 		LastTick;
	uint32_t		SignalTime;
}TBuzzer;

/*
 * Enum for each signal
 */
typedef enum
{
	PRESSED,
	CHANGE_STEP,
	FINISH,
	STOP
}TPlayPressed;

/*
 * Prototypes
 */
void BuzzerInit(TBuzzer* Buzzer,GPIO_TypeDef* GpioPort, uint16_t GpioPin);
void BuzzerPlay(TBuzzer* Buzzer,TPlayPressed PlaySignal);


#endif /* INC_BUZZER_H_ */

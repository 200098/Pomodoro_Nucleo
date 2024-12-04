/*
 * Buzzer.c
 *
 *  The MIT License.
 *  Created on: 2.12.2024
 *      Author: Szymon Frydrych
 *      szfrydrych@gmail.com
 */

#include "main.h"
#include "Buzzer.h"

/*
 * Initialization of buzzer
 */
void BuzzerInit(TBuzzer* Buzzer, GPIO_TypeDef* GpioPort, uint16_t GpioPin)
{
	Buzzer->GpioPort = GpioPort;
	Buzzer->GpioPin = GpioPin;
	HAL_GPIO_WritePin(Buzzer->GpioPort, Buzzer->GpioPin, GPIO_PIN_RESET);
}

/*
 * Play signal by buzzer
 */

void BuzzerPlay(TBuzzer* Buzzer,TPlayPressed PlaySignal)
{
	switch(PlaySignal)
	{
		case PRESSED:
			HAL_GPIO_WritePin(Buzzer->GpioPort, Buzzer->GpioPin, GPIO_PIN_SET);
			Buzzer->LastTick = HAL_GetTick();
			Buzzer->SignalTime = PRESSED_TIME;
			break;
		case CHANGE_STEP:
			HAL_GPIO_WritePin(Buzzer->GpioPort, Buzzer->GpioPin, GPIO_PIN_SET);
			Buzzer->LastTick = HAL_GetTick();
			Buzzer->SignalTime = CHANGE_STEP_TIME;
			break;
		case FINISH:
			HAL_GPIO_WritePin(Buzzer->GpioPort, Buzzer->GpioPin, GPIO_PIN_SET);
			Buzzer->LastTick = HAL_GetTick();
			Buzzer->SignalTime = FINISH_TIME;
			break;
		case STOP:
			if (HAL_GetTick() - Buzzer->LastTick >= Buzzer->SignalTime)
				HAL_GPIO_WritePin(Buzzer->GpioPort, Buzzer->GpioPin, GPIO_PIN_RESET);
			break;
	}
}

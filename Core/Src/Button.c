/*
 * Button.c
 *
 *      Author: Szymon Frydrych
 */
#include "main.h"
#include "stdbool.h"
#include "Button.h"

_Bool Flag_PressedDebounced;

/*
 * Initialization of button
 */
void ButtonInit(TButton* Button, GPIO_TypeDef* GpioPort, uint16_t GpioPin)
{
	Button->State= IDLE_BUTTON;
	Button->GpioPort = GpioPort;
	Button->GpioPin = GpioPin;
	Button->TimerDebounce = DEBOUNCE_TIME;

	Flag_PressedDebounced = false;
}

/*
 * Button State Machine
 */
void ButtonTask(TButton* Button)
{
	switch(Button->State)
	{
		case IDLE_BUTTON:
			if(BUTTON_PRESSED == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))
			{
				Button->LastTick = HAL_GetTick();
				Button->State= DEBOUNCE_BUTTON;

				Flag_PressedDebounced = false;
			}
			break;
		case DEBOUNCE_BUTTON:
			if (HAL_GetTick() - Button->LastTick >= Button->TimerDebounce)
			{
				Button->LastTick = HAL_GetTick();
				if(BUTTON_PRESSED == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))
				{
					Button->State= PRESSED_BUTTON;

					Flag_PressedDebounced = true;
				}
				else
				Button->State= IDLE_BUTTON;
			}
			break;
		case PRESSED_BUTTON:
			if(BUTTON_RELEASED == HAL_GPIO_ReadPin(Button->GpioPort, Button->GpioPin))
			{
				Button->State= IDLE_BUTTON;
			}
			break;
	}
}

/*
 * Read actual state of button
 */

_Bool ButtonIsPressed(void)
{
	if (Flag_PressedDebounced)
	{
		Flag_PressedDebounced = false;

		return true;
	}
	return false;
}

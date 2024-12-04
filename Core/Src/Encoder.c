/*
 * Encoder.c
 *
 *  The MIT License.
 *  Created on: 2.12.2024
 *      Author: Szymon Frydrych
 *      szfrydrych@gmail.com
 */
#include "main.h"
#include "stdbool.h"
#include "Pomodoro_StateMachine.h"
#include "Encoder.h"



TIM_HandleTypeDef htim1;

static int32_t EncoderState = 0;
static _Bool EncoderChange = false;
static _Bool EncoderWorking = false;

/*
 * Initialization of encoder
 */
void EncoderInit(TEncoder* Encoder)
{
	Encoder->State= IDLE_ENCODER;

	Encoder->TimerDebounce = DEBOUNCE_TIME;
}

/*
 * State machine of encoder
 */
void EncoderTask(TEncoder* Encoder)
{
	switch(Encoder->State)
	{
		case IDLE_ENCODER:
			if(EncoderIsWorking())
			{
				HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
				Encoder->State= START_HANDLE_ENCODER;
			}
			break;
		case START_HANDLE_ENCODER:

			Encoder->LastTick = HAL_GetTick();
			Encoder->State= DEBOUNCE_ENCODER;

			break;
		case DEBOUNCE_ENCODER:
			if (HAL_GetTick() - Encoder->LastTick >= Encoder->TimerDebounce)
			{
				 Encoder->State= HANDLE_ENCODER;
			}
			else
			{
				Encoder->State= DEBOUNCE_ENCODER;
			}
			break;
		case HANDLE_ENCODER:

			EncoderSetPosition(htim1.Instance->CNT);
			EncoderChange = true;

			if(!EncoderIsWorking())
			{
				Encoder->State= STOP_HANDLE_ENCODER;
			}
			else
			{
				Encoder->State= START_HANDLE_ENCODER;
			}
			break;
		case STOP_HANDLE_ENCODER:

			HAL_TIM_Encoder_Stop(&htim1, TIM_CHANNEL_ALL);
			Encoder->State= IDLE_ENCODER;

			break;
	}
}

/*
 * Set new value when Pomodoro SM goes to new state
 */
void EncoderSetValue(int Value)
{
	__HAL_TIM_SET_COUNTER(&htim1, Value);
}

/*
 * Update when Encoder has changed
 */

_Bool EncoderHasChange(void)
{
	if(EncoderChange)
	{
		EncoderChange = false;
		return true;
	}
	else
		return false;
}

void EncoderSetPosition(int32_t Value)
{
	EncoderState = Value;
}

int32_t EncoderReadPosition(void)
{
	return EncoderState;
}

/*
 * Setter and Getter for Encoder Working
 */

void EncoderSetWorking(_Bool WorkingFlag)
{
	if(WorkingFlag)
		EncoderWorking = true;
	else
		EncoderWorking = false;
}

_Bool EncoderIsWorking(void)
{
	if(EncoderWorking)
	{
		return true;
	}
	return false;
}


/*
 * Pomodoro_StateMachine.c
 *
 *  The MIT License.
 *  Created on: 2.12.2024
 *      Author: Szymon Frydrych
 *      szfrydrych@gmail.com
 */


#include "main.h"
#include "SSD1306_OLED.h"
#include "GFX_BW.h"
#include "stdio.h"
#include "Pomodoro_StateMachine.h"
#include "Button.h"
#include "Buzzer.h"
#include "Encoder.h"
#include "Pomodoro_Display.h"
#include "stdbool.h"

extern TIM_HandleTypeDef htim3;

TPomodoroDisplayToogleText BackToPomodoroText;
TPomodoroDisplayToogleText ExitFromCycleText;
TPomodoroDisplayToogleText PressToConfirm;
TPomodoroDisplayToogleText BasicSetup;
TPomodoroDisplayToogleText OwnSetup;
TPomodoroDisplayToogleText EditSetup;
TPomodoroDisplayToogleText StartWork;
TPomodoroDisplayToogleText BackToMenu;

TBuzzer Buzzer;

int8_t presentValue = 0;

/*
 * Initialization of Pomodoro
 */
void PomodoroInit(TPomodoro* Pomodoro)
{
	Pomodoro->State			= IDLE;
	Pomodoro->PreviousState	= IDLE;

	Pomodoro->WorkTime 		= 15;

	Pomodoro->RestTime 		= 0;
	Pomodoro->ShortRestTime = 5;
	Pomodoro->LongRestTime 	= 10;

	Pomodoro->LongRestCycle = 4;
	Pomodoro->Cycle    		= 0;

	Pomodoro->RepeatCycle	= 8;
	Pomodoro->ActualTimer  	= 0;
	Pomodoro->TimerBeforePause = 0;


	PomodoroDisplayToggleButtonInit(&BasicSetup,0, 20, 128, 20, 25, 25,  "Basic setup");
	PomodoroDisplayToggleButtonInit(&OwnSetup,0, 40, 128, 20, 25, 45,  "Own setup");

	PomodoroDisplayToggleButtonInit(&EditSetup, 0, 40, 128,10,50,41,  "Edit");
	PomodoroDisplayToggleButtonInit(&StartWork, 0, 50, 128,10,50,51,  "Start");

	PomodoroDisplayToggleButtonInit(&PressToConfirm,0, 50, 128, 20, 30, 55,  "Press to set");

	PomodoroDisplayToggleButtonInit(&BackToPomodoroText, 0, 20, 128, 20, 25, 25,   "Return to cycle");
	PomodoroDisplayToggleButtonInit(&ExitFromCycleText,0, 40, 128, 20, 25, 45,   "Exit cycle");

	PomodoroDisplayToggleButtonInit(&BackToMenu,0, 40, 128, 20, 25, 45,  "Back to menu");

	BuzzerInit(&Buzzer, BUZZER_GPIO_Port, BUZZER_Pin);


}

/*
 * Pomodoro State Machine
 */

void PomodoroTask(TPomodoro* Pomodoro)
{
	BuzzerPlay(&Buzzer,STOP);
	switch(Pomodoro->State)
	{
		case IDLE:
			Pomodoro->State = WAITING_FOR_CHOOSE;
			EncoderSetWorking(true);
			PomodoroDisplayChooseConfiguration();
			break;
		case WAITING_FOR_CHOOSE:
			if(PomodoroDisplayToggleUpdate())
			{
				if(DEVIDED_BY_2(EncoderReadPosition()))
				{
					PomodoroDisplayToggle(&BasicSetup);
					PomodoroDisplaySwitchOffToggle(&OwnSetup);
				}
				else
				{
					PomodoroDisplaySwitchOffToggle(&BasicSetup);
					PomodoroDisplayToggle(&OwnSetup);
				}
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);

				if(DEVIDED_BY_2(EncoderReadPosition()))
				{
					EncoderSetValue(1);
					Pomodoro->State = PRESENT_SETUP;
					PomodoroDisplayPresentSetup(Pomodoro);
				}
				else
				{
					EncoderSetValue(4);
					Pomodoro->State = SET_OWN_CYCLES;
					PomodoroDisplayClearDisplay();
				}
			}
			break;
		case SET_OWN_CYCLES:
			if(EncoderHasChange())
			{

				presentValue = (int)EncoderReadPosition();
				PomodoroDisplayOwnSetting(presentValue, "How much cycles?" , "%2d cycles");
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);
				EncoderSetValue(10);
				Pomodoro->RepeatCycle = presentValue;
				Pomodoro->State = SET_WORK_TIME;
				PomodoroDisplayClearDisplay();
			}
			if(PomodoroDisplayToggleUpdate())
			{
				PomodoroDisplayToggle(&PressToConfirm);
			}
			break;
		case SET_WORK_TIME:
			if(EncoderHasChange())
			{
				presentValue = (int)EncoderReadPosition();

				PomodoroDisplayOwnSetting(presentValue, "Work time?" , "%2d min");
			}
			if(PomodoroDisplayToggleUpdate())
			{
				PomodoroDisplayToggle(&PressToConfirm);
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);
				EncoderSetValue(5);
				Pomodoro->WorkTime = presentValue;
				Pomodoro->State = SET_REST_TIME;
				PomodoroDisplayClearDisplay();
			}
			break;
		case SET_REST_TIME:
			if(EncoderHasChange())
			{
				presentValue = (int)EncoderReadPosition();

				PomodoroDisplayOwnSetting(presentValue, "Short rest time?", "%2d min");
			}
			if(PomodoroDisplayToggleUpdate())
			{
				PomodoroDisplayToggle(&PressToConfirm);
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);
				EncoderSetValue(4);
				Pomodoro->ShortRestTime = presentValue;
				Pomodoro->State = SET_CYCLE_LONG_REST;
				PomodoroDisplayClearDisplay();
			}
			break;
		case SET_CYCLE_LONG_REST:
			if(EncoderHasChange())
			{
				presentValue = (int)EncoderReadPosition();

				PomodoroDisplayOwnSetting(presentValue, "Long rest in each?" , "%2d cycle");
			}
			if(PomodoroDisplayToggleUpdate())
			{
				PomodoroDisplayToggle(&PressToConfirm);
			}
			if(ButtonIsPressed())
			{

				BuzzerPlay(&Buzzer,PRESSED);
				EncoderSetValue(10);
				Pomodoro->LongRestCycle = presentValue;
				Pomodoro->State = SET_LONG_REST_TIME;
				PomodoroDisplayClearDisplay();
			}
			break;
		case SET_LONG_REST_TIME:
			if(EncoderHasChange())
			{
				presentValue = (int)EncoderReadPosition();
				PomodoroDisplayOwnSetting(presentValue, "Time of long rest?", "%2d min");
			}
			if(PomodoroDisplayToggleUpdate())
			{
				PomodoroDisplayToggle(&PressToConfirm);
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);
				EncoderSetValue(1);
				Pomodoro->LongRestTime = presentValue;
				Pomodoro->State = PRESENT_SETUP;
				PomodoroDisplayPresentSetup(Pomodoro);
			}
			break;
		case PRESENT_SETUP:
			if(PomodoroDisplayToggleUpdate())
			{
				if(DEVIDED_BY_2(EncoderReadPosition()))
				{
					PomodoroDisplayToggle(&EditSetup);
					PomodoroDisplaySwitchOffToggle(&StartWork);
				}
				else
				{
					PomodoroDisplaySwitchOffToggle(&EditSetup);
					PomodoroDisplayToggle(&StartWork);
				}
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);
				if(DEVIDED_BY_2(EncoderReadPosition()))
				{
					Pomodoro->State = IDLE;
				}
				else
				{
					Pomodoro->State = START_COUTING;
				}
			}
			break;
		case START_COUTING:
			EncoderSetWorking(false);
			Pomodoro->ActualTimer = 0;
			HAL_TIM_Base_Start_IT(&htim3);
			Pomodoro->State = COUNTING_WORK;
			Pomodoro->Cycle++;
			break;
		case COUNTING_WORK:
			if(PomodoroDisplayUpdate())
			{
				PomodoroDisplayStateOfWork(WORK, Pomodoro);
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);

				Pomodoro->PreviousState = Pomodoro->State;
				Pomodoro->State = PREPERE_TO_PAUSE;
				break;
			}
			if((MIN_TO_SEC(Pomodoro->WorkTime)) == Pomodoro->ActualTimer)
			{
				Pomodoro->ActualTimer = 0;
				if( Pomodoro->RepeatCycle == Pomodoro->Cycle)
				{
					Pomodoro->State = DISPLAY_FINISH;
				}
				else
				{
					BuzzerPlay(&Buzzer,CHANGE_STEP);
					Pomodoro->State = COUNTING_REST;
				  	if(0 == (Pomodoro->Cycle%Pomodoro->LongRestCycle))
				  	{
				  		Pomodoro->RestTime = Pomodoro->LongRestTime;
				  	}
				  	else
				  	{
				  		Pomodoro->RestTime = Pomodoro->ShortRestTime;
				  	}
				}
			}
			break;
		case COUNTING_REST:
			if(PomodoroDisplayUpdate())
			{
				PomodoroDisplayStateOfWork(REST, Pomodoro);
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);
				Pomodoro->PreviousState = Pomodoro->State;
				Pomodoro->State = PREPERE_TO_PAUSE;
				break;
			}
			if((MIN_TO_SEC(Pomodoro->RestTime)) == Pomodoro->ActualTimer)
			{
				Pomodoro->ActualTimer = 0;
				Pomodoro->Cycle++;
				Pomodoro->State = COUNTING_WORK;
				BuzzerPlay(&Buzzer,CHANGE_STEP);
			}
			break;
		case PREPERE_TO_PAUSE:
			Pomodoro->TimerBeforePause = Pomodoro->ActualTimer;
			PomodoroDisplayPause();
			EncoderSetWorking(true);
			EncoderSetValue(0);
			Pomodoro->State = PAUSE;
			break;
		case PAUSE:
			if(PomodoroDisplayToggleUpdate())
			{
				if(DEVIDED_BY_2(EncoderReadPosition()))
			  	{
			  		PomodoroDisplayToggle(&BackToPomodoroText);
			  		PomodoroDisplaySwitchOffToggle(&ExitFromCycleText);
			  	}
			  	else
			  	{
			  		PomodoroDisplaySwitchOffToggle(&BackToPomodoroText);
			  		PomodoroDisplayToggle(&ExitFromCycleText);
			  	}
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);
			  	EncoderSetWorking(false);
			  	if(DEVIDED_BY_2(EncoderReadPosition()))
			  	{
			  		Pomodoro->ActualTimer = Pomodoro->TimerBeforePause;
			  		Pomodoro->State = Pomodoro->PreviousState;
			  	}
			  	else
			  	{
			  		Pomodoro->State = RETURNING_TO_IDLE;
			  	}
			}
			break;
		case DISPLAY_FINISH:
			BuzzerPlay(&Buzzer,FINISH);
			HAL_TIM_Base_Stop_IT(&htim3);
			PomodoroDisplayFinish();
			Pomodoro->State = LEAVE;
			break;
		case LEAVE:
			if(PomodoroDisplayToggleUpdate())
			{
				PomodoroDisplayToggle(&BackToMenu);
			}
			if(ButtonIsPressed())
			{
				BuzzerPlay(&Buzzer,PRESSED);
				Pomodoro->State = RETURNING_TO_IDLE;
			}
			break;
		case RETURNING_TO_IDLE:
			EncoderSetValue(0);
			Pomodoro->State = IDLE;
			Pomodoro->Cycle = 0;
			Pomodoro->ActualTimer = 0;
			break;
	}
}


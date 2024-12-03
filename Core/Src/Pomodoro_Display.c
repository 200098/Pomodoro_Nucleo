/*
 * PomodoroDisplay.c
 *
 *      Author: Szymon Frydrych
 */

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"

#include "SSD1306_OLED.h"
#include "GFX_BW.h"

#include "Pomodoro_StateMachine.h"
#include "Pomodoro_Display.h"


char str[32];

static _Bool Flag_DisplayUpdate;
static _Bool Flag_DisplayToggleUpdate;

/*
 * Functions to set update of display
 */
void PomodoroDisplay_SetUpdate(void)
{

		Flag_DisplayUpdate = true;
}

void PomodoroDisplay_SetToggleUpdate(void)
{
		Flag_DisplayToggleUpdate = true;
}

/*
 * Functions to handle update of display
 */
_Bool PomodoroDisplayUpdate(void)
{
	if (Flag_DisplayUpdate)
	{
		Flag_DisplayUpdate = false;
		return true;
	}
	return false;
}

_Bool PomodoroDisplayToggleUpdate(void)
{
	if (Flag_DisplayToggleUpdate)
	{
		Flag_DisplayToggleUpdate = false;
		return true;
	}
	return false;
}

/*
 * Functions for carry of Toggle button
 */

void PomodoroDisplayToggleButtonInit(TPomodoroDisplayToogleText *ToggledText, const uint8_t xPosition, const uint8_t yPosition,
		const uint8_t xSize, const uint8_t ySize, const uint8_t xText, const uint8_t yText,char* text)
{

	ToggledText->xPosition = xPosition;
	ToggledText->yPosition = yPosition;

	ToggledText->xSize = xSize;
	ToggledText->ySize = ySize;

	ToggledText->xText = xText;
	ToggledText->yText = yText;
	ToggledText->text = text;

}

void PomodoroDisplaySwitchOffToggle(TPomodoroDisplayToogleText *ToggledText)
{
	GFX_DrawFillRectangle(0, ToggledText->yPosition, ToggledText->xSize, ToggledText->ySize, BLACK);
	GFX_DrawString(ToggledText->xText , ToggledText->yText, ToggledText->text , WHITE, BLACK);
	SSD1306_Display();
}

void PomodoroDisplayToggle(TPomodoroDisplayToogleText *ToggledText)
{
	static TMode actualMode = DARK;

	switch(actualMode)
		{
			case DARK:
				actualMode = BRIGHT;
				GFX_DrawFillRectangle(0, ToggledText->yPosition, ToggledText->xSize, ToggledText->ySize, BLACK);
				GFX_DrawString(ToggledText->xText , ToggledText->yText, ToggledText->text , WHITE, BLACK);

			break;
			case BRIGHT:
				actualMode = DARK;
				GFX_DrawFillRectangle(0, ToggledText->yPosition, ToggledText->xSize, ToggledText->ySize, WHITE);
				GFX_DrawString(ToggledText->xText , ToggledText->yText, ToggledText->text ,BLACK, WHITE);
			break;
		}
		SSD1306_Display();
}

/*
 * Functions to present Screen in each stare
 */

void PomodoroDisplayChooseConfiguration(void)
{
	PomodoroDisplayClearDisplay();
	GFX_DrawString(10, 10, "Choose your setup" , WHITE, BLACK);
	SSD1306_Display();
}

void PomodoroDisplayOwnSetting(const int8_t presentValue, char* question, char* unit)
{
	GFX_DrawString(10, 10, question , WHITE, BLACK);
	sprintf(str, unit, presentValue);
	GFX_DrawString(40, 25, str , WHITE, BLACK);
	GFX_DrawString(10, 40, "Use knob to change" , WHITE, BLACK);

	SSD1306_Display();
}

void PomodoroDisplayPresentSetup(TPomodoro* Pomodoro)
{
	PomodoroDisplayClearDisplay();
	sprintf(str, "Cycles of pomodoro: %d", (int)Pomodoro->RepeatCycle);
	GFX_DrawString(00, 00, str , WHITE, BLACK);

	sprintf(str, "Work:%dmin", (int)Pomodoro->WorkTime);
	GFX_DrawString(00, 10, str , WHITE, BLACK);

	sprintf(str, "Rest:%dmin", (int)Pomodoro->ShortRestTime);
	GFX_DrawString(67, 10, str , WHITE, BLACK);

	sprintf(str, "Break:%dmin", (int)Pomodoro->LongRestTime);
	GFX_DrawString(00, 20, str , WHITE, BLACK);

	sprintf(str, "in every %d cycle", (int)Pomodoro->LongRestCycle);
	GFX_DrawString(00, 30, str , WHITE, BLACK);

	SSD1306_Display();
}

void PomodoroDisplayStateOfWork(TStateOfWork StateToDisplay, TPomodoro* Pomodoro)
{

	PomodoroDisplayClearDisplay();
	uint8_t minute;
	uint8_t seconds;
	uint8_t restTime;

	minute = Pomodoro->ActualTimer/60;
	seconds = Pomodoro->ActualTimer%60;

	GFX_DrawString(10, 00, "You are in cycle" , WHITE, BLACK);
	sprintf(str, " %d of %d cycles", (int)Pomodoro->Cycle, (int)Pomodoro->RepeatCycle);
	GFX_DrawString(10, 10, str , WHITE, BLACK);

	GFX_DrawRectangle(0, 22, 124, 26, WHITE);
	if(WORK == StateToDisplay)
	{
		GFX_DrawString(20, 25, "Time to work" , WHITE, BLACK);
		restTime = Pomodoro->WorkTime;
	}

	else
	{
		GFX_DrawString(20, 25, "Time for rest" , WHITE, BLACK);
		restTime = Pomodoro->RestTime;
	}

	sprintf(str, "%1d:%02d min in %2d min", minute,seconds,restTime);
	GFX_DrawString(10, 35, str , WHITE, BLACK);

	GFX_DrawString(20, 50, "PRESS TO PAUSE" , WHITE, BLACK);
	SSD1306_Display();

}

void PomodoroDisplayPause(void)
{
	PomodoroDisplayClearDisplay();
	GFX_DrawString(50 , 11, "Pause" , WHITE, BLACK);
	SSD1306_Display();
}


void PomodoroDisplayFinish(void)
{
	PomodoroDisplayClearDisplay();
	GFX_DrawString(10, 10, "Completed all cycles" , WHITE, BLACK);
	SSD1306_Display();
}

void PomodoroDisplayClearDisplay(void)
{
	SSD1306_Clear(BLACK);
}


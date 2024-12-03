/*
 * PomodoroDisplay.h
 *
 *      Author: Szymon Frydrych
 */

#ifndef INC_POMODORO_DISPLAY_H_
#define INC_POMODORO_DISPLAY_H_

/*
 * Definitions
 */

/*
 * Struct of Toggle Text
 */

typedef struct
{

	uint8_t 		xPosition;
	uint8_t 		yPosition;
	uint8_t 		xSize;
	uint8_t 		ySize;
	uint8_t 		xText;
	uint8_t 		yText;

	char* 			text;

}TPomodoroDisplayToogleText;


/*
 * Enum for each mode
 */
typedef enum mode
{
	BRIGHT,
	DARK
}TMode;

/*
 * Enum for state of cycle
 */
typedef enum
{
	WORK,
	REST
}TStateOfWork;

/*
 * Prototypes
 */

void PomodoroDisplay_SetUpdate(void);

void PomodoroDisplay_SetToggleUpdate(void);

_Bool PomodoroDisplayUpdate(void);

_Bool PomodoroDisplayToggleUpdate(void);


void PomodoroDisplayToggleButtonInit(TPomodoroDisplayToogleText *ToggledText, const uint8_t xPosition, const uint8_t yPosition,
		const uint8_t xSize, const uint8_t ySize, const uint8_t xText, const uint8_t yText,char* text);

void PomodoroDisplaySwitchOffToggle(TPomodoroDisplayToogleText *ToggledText);

void PomodoroDisplayToggle(TPomodoroDisplayToogleText *ToggledText);

void PomodoroDisplayChooseConfiguration(void);

void PomodoroDisplayOwnSetting(const int8_t presentValue, char* question, char* unit);

void PomodoroDisplayPresentSetup(TPomodoro* Pomodoro);

void PomodoroDisplayStateOfWork(TStateOfWork StateToDisplay, TPomodoro* Pomodoro);

void PomodoroDisplayPause(void);

void PomodoroDisplayFinish(void);

void PomodoroDisplayClearDisplay(void);

#endif /* INC_POMODORO_DISPLAY_H_ */

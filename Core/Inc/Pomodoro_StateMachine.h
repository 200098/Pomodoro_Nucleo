/*
 * Pomodoro_StateMachine.h
 *
 *      Author: Szymon Frydrych
 */

#ifndef INC_POMODORO_STATEMACHINE_H_
#define INC_POMODORO_STATEMACHINE_H_

/*
 * Macros
 */
#define DEVIDED_BY_2(X) 0 == (X)%2
#define MIN_TO_SEC(X) X * 60

/*
 * Pomodoro's states
 */
typedef enum
{
	IDLE = 0,
	WAITING_FOR_CHOOSE,
	SET_OWN_CYCLES,
	SET_WORK_TIME,
	SET_REST_TIME,
	SET_CYCLE_LONG_REST,
	SET_LONG_REST_TIME,
	PRESENT_SETUP,
	START_COUTING,
	COUNTING_WORK,
	COUNTING_REST,
	PREPERE_TO_PAUSE,
	PAUSE,
	DISPLAY_FINISH,
	LEAVE,
	RETURNING_TO_IDLE,
}TPomodoroState;

/*
 * Struct of Pomodoro
 */
typedef struct
{
	TPomodoroState 	State;
	TPomodoroState  PreviousState;


	uint8_t 		WorkTime;
	uint8_t 		RestTime;
	uint8_t 		ShortRestTime;
	uint8_t 		LongRestTime;
	uint8_t			TimeBeforePause;
	uint8_t 		LongRestCycle;
	uint8_t			Cycle;
	uint8_t			RepeatCycle;

	uint16_t		ActualTimer;
	uint16_t		TimerBeforePause;

}TPomodoro;

/*
 * Prototypes
 */
void PomodoroInit(TPomodoro* Pomodoro);

void PomodoroTask(TPomodoro* Pomodoro);


#endif /* INC_POMODORO_STATEMACHINE_H_ */

/*
 * Encoder.h
 *
 *  The MIT License.
 *  Created on: 2.12.2024
 *      Author: Szymon Frydrych
 *      szfrydrych@gmail.com
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

/*
 * Definitions
 */

#define DEBOUNCE_TIME 50

/*
 * Encoder's states
 */
typedef enum
{
	IDLE_ENCODER = 0,
	START_HANDLE_ENCODER,
	DEBOUNCE_ENCODER,
	HANDLE_ENCODER,
	STOP_HANDLE_ENCODER,
}TEncoderState;

/*
 * Struct of Encoder
 */
typedef struct
{
	TEncoderState 	State;

	uint32_t 		LastTick;
	uint32_t 		TimerDebounce;

}TEncoder;

/*
 * Prototypes
 */
void EncoderInit(TEncoder* Encoder);

void EncoderTask(TEncoder* Encoder);

void EncoderSetValue(int Value);

_Bool EncoderHasChange(void);

void EncoderSetPosition(int32_t Value);

int32_t EncoderReadPosition(void);

void EncoderSetWorking(_Bool WorkingFlag);

_Bool EncoderIsWorking(void);

#endif /* INC_ENCODER_H_ */

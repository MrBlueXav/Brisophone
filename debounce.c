/**
 ******************************************************************************
 * File Name		: debounce.c
 * Author			: Xavier Halgand
 * Date				: 24 juillet 2013
 * Description		: from Jack Ganssle (http://www.ganssle.com/)
 ******************************************************************************
 */
#include "debounce.h"

/* Defines for push buttons ---------------------------------------------------*/
#define BUT1	GPIO_Pin_8
#define BUT2	GPIO_Pin_9
#define BUT3	GPIO_Pin_10
#define BUT4	GPIO_Pin_11

/*--------------------------------------------------------------------------------------------------------------------------------------*/

static uint8_t Count[4] = {RELEASE_MSEC / CHECK_MSEC, RELEASE_MSEC / CHECK_MSEC, RELEASE_MSEC / CHECK_MSEC, RELEASE_MSEC / CHECK_MSEC};
static bool DebouncedKeyPress[4] = {false, false, false, false};  // This holds the debounced state of the key.
static bool Key_changed[4] = {false, false, false, false};
static bool Key_pressed[4] = {false, false, false, false};

/***************************************************************************************************************************/
// Service routines called every CHECK_MSEC to
// debounce both edges

void DebounceSwitch1(void) // Called by SysTick_Handler() in file stm32f4xx_it.c
{
	bool RawState;

	RawState = !((bool)PBGetState(BUT1));

	if (RawState == DebouncedKeyPress[0]) {
		// Set the timer which allows a change from current state.
		if (DebouncedKeyPress[0]) Count[0] = RELEASE_MSEC / CHECK_MSEC;
		else Count[0] = PRESS_MSEC / CHECK_MSEC;

	} else {
		// Key has changed - wait for new state to become stable.
		if (--Count[0] == 0) {
			// Timer expired - accept the change.
			DebouncedKeyPress[0] = RawState;
			Key_changed[0] = true;
			Key_pressed[0] = DebouncedKeyPress[0];
			// And reset the timer.
			if (DebouncedKeyPress[0]) // rising edge
			{
				Count[0] = RELEASE_MSEC / CHECK_MSEC;
				action_ButtonPressed1();
			}
			else // falling edge
			{
				Count[0] = PRESS_MSEC / CHECK_MSEC;
				action_ButtonReleased1();
			}
		}
	}
}

void DebounceSwitch2(void) // Called by SysTick_Handler() in file stm32f4xx_it.c
{
	bool RawState;

	RawState = !((bool)PBGetState(BUT2));

	if (RawState == DebouncedKeyPress[1]) {
		// Set the timer which allows a change from current state.
		if (DebouncedKeyPress[1]) Count[1] = RELEASE_MSEC / CHECK_MSEC;
		else Count[1] = PRESS_MSEC / CHECK_MSEC;

	} else {
		// Key has changed - wait for new state to become stable.
		if (--Count[1] == 0) {
			// Timer expired - accept the change.
			DebouncedKeyPress[1] = RawState;
			Key_changed[1] = true;
			Key_pressed[1] = DebouncedKeyPress[1];
			// And reset the timer.
			if (DebouncedKeyPress[1]) // rising edge
			{
				Count[1] = RELEASE_MSEC / CHECK_MSEC;
				action_ButtonPressed2();
			}
			else // falling edge
			{
				Count[1] = PRESS_MSEC / CHECK_MSEC;
				action_ButtonReleased2();
			}
		}
	}
}

void DebounceSwitch3(void) // Called by SysTick_Handler() in file stm32f4xx_it.c
{
	bool RawState;

	RawState = !((bool)PBGetState(BUT3));

	if (RawState == DebouncedKeyPress[2]) {
		// Set the timer which allows a change from current state.
		if (DebouncedKeyPress[2]) Count[2] = RELEASE_MSEC / CHECK_MSEC;
		else Count[2] = PRESS_MSEC / CHECK_MSEC;

	} else {
		// Key has changed - wait for new state to become stable.
		if (--Count[2] == 0) {
			// Timer expired - accept the change.
			DebouncedKeyPress[2] = RawState;
			Key_changed[2] = true;
			Key_pressed[2] = DebouncedKeyPress[2];
			// And reset the timer.
			if (DebouncedKeyPress[2]) // rising edge
			{
				Count[2] = RELEASE_MSEC / CHECK_MSEC;
				action_ButtonPressed3();
			}
			else // falling edge
			{
				Count[2] = PRESS_MSEC / CHECK_MSEC;
				action_ButtonReleased3();
			}
		}
	}
}

void DebounceSwitch4(void) // Called by SysTick_Handler() in file stm32f4xx_it.c
{
	bool RawState;

	RawState = !((bool)PBGetState(BUT4));

	if (RawState == DebouncedKeyPress[3]) {
		// Set the timer which allows a change from current state.
		if (DebouncedKeyPress[3]) Count[3] = RELEASE_MSEC / CHECK_MSEC;
		else Count[3] = PRESS_MSEC / CHECK_MSEC;

	} else {
		// Key has changed - wait for new state to become stable.
		if (--Count[3] == 0) {
			// Timer expired - accept the change.
			DebouncedKeyPress[3] = RawState;
			Key_changed[3] = true;
			Key_pressed[3] = DebouncedKeyPress[3];
			// And reset the timer.
			if (DebouncedKeyPress[3]) // rising edge
			{
				Count[3] = RELEASE_MSEC / CHECK_MSEC;
				action_ButtonPressed4();
			}
			else // falling edge
			{
				Count[3] = PRESS_MSEC / CHECK_MSEC;
				action_ButtonReleased4();
			}
		}
	}
}

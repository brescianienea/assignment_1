#ifndef __BUTTON_MANAGER_H__
#define __BUTTON_MANAGER_H__

#include "Arduino.h"

#define BUTTON1 9
#define BUTTON2 10
#define BUTTON3 11
#define BUTTON4 12

extern const int8_t buttons[];

/// @brief Initialize buttons as input.
void buttonsInit();

/// @brief Buttons number getter.
/// @return the number of buttons.
int getButtonsNumber();

/// @brief Attach interrupts to each button.
void activateButtonsGameInterrupt();

/// @brief Detach interrupts from each button.
void deactivateButtonsGameInterrupt();

/// @brief Check if has been pressed the correct button.
/// @param buttonPin button pin to check.
/// @return true if the button was correct, false if it was wrong.
bool buttonPressed(int8_t buttonPin);

/// @brief Check if the interrupt is probably due to a bouncing problem
/// @return true if there are no bouncing problem, false if is probably bouncing
bool checkBouncing();

/// @brief Activate deep sleep mode and set the interrupt to wake up the system for each button
void sleepNow();

/// @brief Empty function for wake up interrupt
void wakeUpNow();

#endif

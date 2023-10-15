#include <Arduino.h>
#include "main.h"
#include "game_system.h"
#include "led_manager.h"
#include "time_utility.h"
#include "button_manager.h"
#include "potenziometer_manager.h"

/// Define the current state of the game
GameState gameState = SETUP;

/// Used to print setup message only one time a game
bool printSetupMessage = true;

/// Variables managing timers for events
unsigned long sleepModeStartTime;
unsigned long switchOnGreenLedsStartTime;
unsigned long switchOffGreenLedStartTime;
unsigned long gameOverStartTime;

/// Player have 10s to start the game or the system go in deep sleep mode
const unsigned long sleepModeTime = 10000;

/// Variable used on not blocking delays
unsigned long prevTime = 0;

/// State of the MC phase, true if still have to light up some leds, false otherwise
bool ledsTurningOn = true;

void setup() {
    sleepModeStartTime = millis();
    Serial.begin(9600);
}

void loop() {
    switch (gameState)
    {
    case SETUP:
        if (printSetupMessage) {
            Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
            switchGreenLeds(false);
            ledFading(RED_LED);
            printSetupMessage = false;
        }

        basicTimer(sleepModeTime, &sleepModeStartTime, sleepNowTrampoline);
        
        if (digitalRead(BUTTON1) == HIGH) {
            switchOnGreenLedsStartTime = millis();
            gameInit(getDifficulty());
            gameState = LEDS_ON;
            Serial.println("GO!");
        }
        break;
    case INITIALIZATION:
        break;
    case LEDS_ON:
        basicTimer(T1, &switchOnGreenLedsStartTime, switchGreenLeds, true);

        if (getGreenLeds() == getGreenLedsOn()) {
            gameState = LEDS_OFF;
        }
        
        break;
    case LEDS_OFF:
        basicTimer(currentT2, &switchOffGreenLedStartTime, switchRandomLedOff);

        // Check if the MC phase is finished
        if (getGreenLedsOn() == 0) {
            //Initialize the PLAYER state
            activateButtonsGameInterrupt();
            gameOverStartTime = millis();
            //Change to PLAYER state            
            gameState = PLAYER;
        }

        break;
    case PLAYER:
        /* here check if the player pressed wrong button */
        /* here check if the player win the game */
        /* this is one of the last function to launch in this state */
        basicTimer(currentT3, &gameOverStartTime, setGameOver);
        /* this is the last function to launch in this state */
        deactivateButtonsGameInterrupt();
        break;
    case NEWLEVEL:
        break;
    case GAMEOVER:
        printFinalScore();
        gameOver();
        gameState = SETUP;
        printSetupMessage = true;
        break;
    default:
        break;
    }
}

void sleepNowTrampoline() {
    Serial.println("Sleep mode actived");
    switchGreenLeds(false);
    switchLed(RED_LED, false);
    sleepNow();
}

void setGameOver() {
    gameState = GAMEOVER;
}
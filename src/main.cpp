#include <Arduino.h>
#include "game_system.h"
#include "led_menager.h"
#include "sleep_mode_utility.h"
#include "button_manager.h"
#include "game_over.h"

#define TEN_SECONDS 10000
#define THREE_SECONDS 3000
GameState gameState = SETUP;

/**
 * This timer launch method after a certain time.
 * @param time time in milliseconds
 * @param function function to launch
 * @param s is the function's parameter 
*/
void basicTimer(unsigned long time, void (*function)(boolean), boolean s);

/**
 * Print a string one time if the field 'printed' is false.
 * If you wont to print the string again, you have to set the field 'printed' to false.
 * @param s string to print
*/
void printStringOneTime(String s);

static boolean printed = false;

unsigned long sleepModeStartTime = millis();
unsigned long T1;
unsigned long T2;
unsigned long T3;
unsigned long points;
/**
 * Player have 10s to start the game or the system go in deep sleep mode.
*/
unsigned long sleepModeTime = TEN_SECONDS;

void setup() {
    led_init_output();
    Serial.begin(9600);
}

void loop() {
    switch (gameState)
    {
    case SETUP:
        printStringOneTime("Welcome to the Restore the Light Game. Press Key B1 to Start");
        switchGreenLeds(false);
        ledFading(RED_LED, true);
        basicTimer(TEN_SECONDS, sleepNowTrampoline, true);
        break;
    case MC:
        basicTimer(THREE_SECONDS, switchGreenLeds, true);

        /* this is one of the last function to launch in this state */
        activateButtonsGameInterrupt();
        break;
    case PLAYER:
        /* here check if the player pressed wrong button */
        /* here check if the player win the game */
        /* this is one of the last function to launch in this state */
        basicTimer(T3, gameOver, true);
        break;
    case NEWLEVEL:
        break;
    case GAMEOVER:
        gameOver(points);
        gameState = SETUP;
        break;
    default:
        break;
    }
    delay(1000);
}

void basicTimer(unsigned long time, void (*function)(boolean), boolean s) {
    if (millis() - sleepModeStartTime >= time) {
        Serial.println("Switch on green leds");
        function(s);
    }
}

static void sleepNowTrampoline(boolean s) {
    sleepNow();
}

void printStringOneTime(String s) {
    if (!printed) {
        Serial.println(s);
        printed = true;
    }
}

void gameOver(boolean s) {
    gameState = GAMEOVER;
}

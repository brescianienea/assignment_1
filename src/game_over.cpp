#include <Arduino.h>
#include "game_over.h"

int gameOver(int points){
    Serial.println(points);
    int startingTime = millis();
    //arduino led fade from light on to light off over 10 sec
    analogWrite(LED_PIN, 255);
    for (; millis() - startingTime < 10000;) {
        analogWrite(LED_PIN, map(millis() - startingTime, 0, 10000, 255, 0));
    }
    analogWrite(LED_PIN, 0);
    return 0;
}
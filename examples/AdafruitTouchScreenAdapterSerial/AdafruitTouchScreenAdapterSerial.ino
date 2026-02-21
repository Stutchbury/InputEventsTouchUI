/**
 * @file AdafruitTouchScreenAdapterSerial.ino
 * @author Philip Fletcher (philip.fletcher@stutchbury.com)
 * @brief 
 * @version 0.1
 * @date 2025-09-13
 * 
 * @copyright Copyright (c) 2025
 * 
 * Use this example to test the configuration of your TouchScreenAdapter
 * 
 */

#include <Arduino.h>

#include <TouchScreen.h> //Adafruit's TouchScreen library

#include "TouchScreenAdapter/AdafruitResistiveTouchScreenAdapter.h" // The adapter

// Use Adafruit TouchScreen example defines for easy cut'n'paste ;-)
// #define YP A2  // must be an analog pin, use "An" notation!
// #define XM A3  // must be an analog pin, use "An" notation!
// #define YM 10 //8   // can be a digital pin
// #define XP 9   // can be a digital pin

#define XP A7  // Y+ must be an analog pin, use "An" notation!
#define YP A6  // X- must be an analog pin, use "An" notation!
#define XM A9  // Y- can be a digital pin
#define YM A8  // X+ can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, it's 300 ohms across the X plate
input_events::AdafruitResistiveTouchScreenAdapter touchAdapter(XP, YP, XM, YM, 300); //Using default 240 x 320 display

// Use a custom display size. Note, this is the raw (unrotated size, likely to be portrait).
//input_events::AdafruitResistiveTouchScreenAdapter touchAdapter(XP, YP, XM, YM, 300, 240, 320);


// Just to slow down the serial output on touch
const uint16_t rateLimitMs = 10; 
uint32_t rateLimitTimer = 0;

void setup() {

    Serial.begin(9600);
    touchAdapter.begin();
    uint8_t rotation = 0; //0-3
    touchAdapter.setRotation(rotation);

    //Adjust the edges for the resistive touch panel
    touchAdapter.setMinRawX(110);
    touchAdapter.setMaxRawX(870);
    touchAdapter.setMinRawY(150);
    touchAdapter.setMaxRawY(870);
    // //And the raw touch pressure
    touchAdapter.setMinRawZ(20);
    touchAdapter.setMaxRawZ(1000);

    delay(1000);

    Serial.println("Starting TouchScreenAdapter Serial Example...");
}

void loop() {
    if ( millis() > rateLimitTimer + rateLimitMs ) {
        rateLimitTimer = millis();
        input_events::TouchPoint_s tpr = touchAdapter.getTouchPointRaw();
        input_events::TouchPoint_s tp = touchAdapter.getTouchPoint();
        if (tp.z > 0 ) {
            Serial.print("Touched X: ");
            Serial.print(tp.x);
            Serial.print(",\tY: ");
            Serial.print(tp.y);
            Serial.print(",\tZ: ");
            Serial.print(tp.z);
            Serial.print("\tRaw X: ");
            Serial.print(tpr.x);
            Serial.print(",\tY: ");
            Serial.print(tpr.y);
            Serial.print(",\tZ: ");
            Serial.print(tpr.z);
            Serial.println();
        }
    }
}


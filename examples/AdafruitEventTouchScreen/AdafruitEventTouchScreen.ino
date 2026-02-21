#include <Arduino.h>

#include <TouchScreen.h> //Adafruit's TouchScreen library

#include "TouchScreenAdapter/AdafruitResistiveTouchScreenAdapter.h" // The adapter

#include "EventTouchScreen.h"

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

input_events::EventTouchScreen touchScreen(&touchAdapter);

//A utility function to print the event name
void printEvent(InputEventType iet) {
  switch (iet) {
  case InputEventType::ENABLED :
    Serial.print("ENABLED");
    break;
  case InputEventType::DISABLED :
    Serial.print("DISABLED");
    break;
  case InputEventType::IDLE :
    Serial.print("IDLE");
    break;
  case InputEventType::PRESSED :
    Serial.print("PRESSED");
    break;
  case InputEventType::RELEASED :
    Serial.print("RELEASED");
    break;
  case InputEventType::CLICKED :
    Serial.print("CLICKED");
    break;
  case InputEventType::DOUBLE_CLICKED :
    Serial.print("DOUBLE_CLICKED");
    break;
  case InputEventType::MULTI_CLICKED :
    Serial.print("MULTI_CLICKED");
    break;
  case InputEventType::LONG_CLICKED :
    Serial.print("LONG_CLICKED");
    break;
  case InputEventType::LONG_PRESS :
    Serial.print("LONG_PRESS");
    break;
  case InputEventType::DRAGGED :
    Serial.print("DRAGGED");
    break;
  case InputEventType::DRAGGED_RELEASED :
    Serial.print("DRAGGED_RELEASED");
    break;
  default:
    break;  
  }
}

// The callback function
void onTouchScreenEvent(InputEventType et, input_events::EventTouchScreen & ts) {
    input_events::TouchPoint_s tp = ts.getTouchPoint();
    //input_events::TouchPoint_s ptp = ts.getPreviousTouchPoint();
    Serial.print("onTouchScreenEvent: ");
    printEvent(et);
    //TouchScreenAdapter::TouchPoint ptp = ts.getPreviousTouchPoint();
    //Serial.printf(" Previous X: %3i, Y: %3i, Z: %3i, Current X: %3i, Y: %3i Z: %3i \n", ptp.x, ptp.y, ptp.z, tp.x, tp.y, tp.z);
    Serial.print( " X: ");
    Serial.print(tp.x);
    Serial.print( ", Y: ");
    Serial.print(tp.y);
    Serial.println();
}


// Just to slow down the serial output on touch
const uint16_t rateLimitMs = 10; 
uint32_t rateLimitTimer = 0;

void setup() {

    Serial.begin(9600);
    touchAdapter.begin();
    touchScreen.begin();

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

    touchScreen.setCallback(onTouchScreenEvent);

    delay(1000);

    Serial.println("Starting TouchScreenAdapter Serial Example...");
}

void loop() {
    touchScreen.update();
}


#include <Arduino.h>

#include "Adafruit_GFX.h"
//Use the GFX sub class appropriate for your display
#include "Adafruit_ILI9341.h"

//Include the underlying ToushScreen library
#include <TouchScreen.h>
//Then the Adapter for it
#include "TouchScreenAdapter/AdafruitResistiveTouchScreenAdapter.h"

#include "EventTouchScreen.h"

// Using Adafuit defines for easy cut'n'paste!
#define YP A6  // must be an analog pin, use "An" notation!
#define XM A9  // must be an analog pin, use "An" notation!
#define YM 22 //A8   //8   // can be a digital pin
#define XP 21 //A7   // can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, it's 300 ohms across the X plate
const uint16_t TOUCHSCREEN_OHMS = 300;

//By default, display is portrait
const uint16_t MY_DISPLAY_WIDTH = ILI9341_TFTWIDTH; //240
const uint16_t MY_DISPLAY_HEIGHT = ILI9341_TFTHEIGHT; //320

const uint8_t TFT_DC = 9; //6; //Change to suit your board
const uint8_t TFT_CS = 10; // 5; //Change to suit your board

//Create the GFX display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//Create the TouchScreenAdapter for Adafruit's TouchScreen
AdafruitResistiveTouchScreen touchAdapter(XP, YP, XM, YM, TOUCHSCREEN_OHMS);

//Create the EventTouchScren, passing the TouchScreenAdapter
EventTouchScreen touchScreen(&touchAdapter);

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
void onTouchScreenEvent(InputEventType et, EventTouchScreen & ts) {
    TouchScreenAdapter::TouchPoint tp = ts.getTouchPoint();
    TouchScreenAdapter::TouchPoint ptp = ts.getPreviousTouchPoint();
    if ( et == InputEventType::PRESSED ) {
      tft.fillCircle(ptp.x, ptp.y, 30, ILI9341_BLACK);
      tft.fillCircle(tp.x, tp.y, 30, ILI9341_WHITE);
  } else if ( et == InputEventType::RELEASED || et == InputEventType::DRAGGED_RELEASED ) {
      tft.fillCircle(ptp.x, ptp.y, 26, ILI9341_BLACK);
  } else if ( et == InputEventType::DRAGGED ) {
      tft.fillCircle(ptp.x, ptp.y, 30, ILI9341_BLACK);
      tft.fillCircle(tp.x, tp.y, 30, ILI9341_WHITE);
  } else if ( et == InputEventType::IDLE ) {
      tft.fillScreen(ILI9341_BLACK);
  }

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


void setup() {
    Serial.begin(9600);
    tft.begin();
    touchScreen.begin();

    touchAdapter.setDisplayWidth(MY_DISPLAY_WIDTH);
    touchAdapter.setDisplayHeight(MY_DISPLAY_HEIGHT);

    //For ResistiveTouch - adjust these values
    touchAdapter.setMinRawY(160);
    touchAdapter.setMaxRawY(900);
    touchAdapter.setMinRawX(100);
    touchAdapter.setMaxRawX(905);
    touchAdapter.setMinRawZ(200);
    touchAdapter.setMaxRawZ(400);

    tft.fillScreen(ILI9341_BLACK);

    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.println("<^Top Left");

    touchScreen.setCallback(onTouchScreenEvent);
    //touchScreen.enableDragging(true); //Comment out (or pass false) to disable dragging and enable LONG_PRESS

    delay(1000); //For Serial
    Serial.println("Starting EventTouchScreen for Adafruit resistive TouchScreen...");
}

void loop() {
    touchScreen.update();
    TouchScreenAdapter::TouchPoint tpr = touchAdapter.getTouchPointRaw();
    //if (tpr.x != 0 || tpr.y != 0 ) {
    if (tpr.z != 0 ) {
        Serial.print("Raw X: ");
        Serial.print(tpr.x);
        Serial.print(". Y: ");
        Serial.print(tpr.y);
        Serial.print(". Z: ");
        Serial.print(tpr.z);
        Serial.println();
    }
}

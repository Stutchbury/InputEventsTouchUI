
#include <Arduino.h>

#include "Adafruit_GFX.h"
//Use the GFX sub class appropriate for your display
#include "Adafruit_ILI9341.h"

//Include the underlying ToushScreen library
#include <Adafruit_FT6206.h>
//Then the Adapter for it
#include "TouchScreenAdapter/AdafruitFT6206TouchScreenAdapter.h"

#include "EventTouchScreen.h"

//By default, display is portrait
const uint16_t MY_DISPLAY_WIDTH = ILI9341_TFTWIDTH; //240
const uint16_t MY_DISPLAY_HEIGHT = ILI9341_TFTHEIGHT; //320

// const uint8_t MY_TFT_DC = 9; //6; //Change to suit your board
// const uint8_t MY_TFT_CS = 10; // 5; //Change to suit your board
const uint8_t MY_TFT_DC = 6; //Change to suit your board
const uint8_t MY_TFT_CS = 5; //Change to suit your board

using namespace input_events;

//Create the GFX display
Adafruit_ILI9341 tft = Adafruit_ILI9341(MY_TFT_CS, MY_TFT_DC);

//Create the TouchScreenAdapter for Adafruit's TouchScreen
input_events::AdafruitFT6206TouchScreenAdapter touchAdapter;

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
    TouchPoint_s tp = ts.getTouchPoint();
    TouchPoint_s ptp = ts.getPreviousTouchPoint();
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
    TouchPoint_s tp = touchAdapter.getTouchPoint();
    TouchPoint_s tpr = touchAdapter.getTouchPoint();
    //if (tpr.x != 0 || tpr.y != 0 ) {
    if (tp.z != 0 ) {
        Serial.print("Raw X: ");
        Serial.print(tpr.x);
        Serial.print(". Y: ");
        Serial.print(tpr.y);
        Serial.print(". Z: ");
        Serial.print(tpr.z);
        Serial.println();
    }
}

/**
 *
 * GPLv2 Licence httouchPoints://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventTouchScreen.h"

namespace input_events {

EventTouchScreen::EventTouchScreen(ITouchScreenAdapter *touchAdapter) :
    touchAdapter(touchAdapter)
    { }

void EventTouchScreen::begin() {
    touchAdapter->begin();
    //Allow the touch panel to settle on startup.
    rateLimitCounter = millis()+500;
}

void EventTouchScreen::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventTouchScreen::update() {
    if (_enabled) {
        if( millis() > (rateLimitCounter + rateLimit) ) { 
            rateLimitCounter = millis();
            if ( debounced()) {
                if ( touchPoint.z != 0 ) {
                    lastTouchedPoint = touchPoint;
                }
                if ( !touched && touchPoint.z != 0 ) {
                    changeState(true);
                    startTouchPoint = touchPoint;
                    previousTouchPoint = touchPoint;
                    lastDragMs = lastStateChange;
                    invoke(InputEventType::PRESSED);
                } else if ( touched && touchPoint.z == 0 ) {
                    changeState(false);
                    if ( !dragging ) {
                        //Serial.printf("Released touchPoint X: %3i, Y: %3i, Z: %3i \n", touchPoint.x, touchPoint.y, touchPoint.x);
                        clickFired = false;
                        if ( longPressCounter == 0 ) {
                            clickCounter++;
                            prevClickCount = clickCounter;
                        }
                        invoke(InputEventType::RELEASED);
                    } else {
                        clickFired = true; //Stop any clicks firing
                        dragging = false;
                        longPressCounter = 0;
                        clickCounter = 0;
                        //Resistive screens tend to press/release after dragged
                        //so 'block' the screen for a bit
                        rateLimitCounter = millis()+postDragRateLimit;
                        invoke(InputEventType::DRAGGED_RELEASED);
                    }
                }
            }
            if ( touched && touchPoint.z != 0 ) {
                resetIdleTimer();
                if ( dragEnabled ) {
                    if ( haveDragged() ) {
                        invoke(InputEventType::DRAGGED);
                        previousTouchPoint = touchPoint;
                    }
                }
                if ( (currentDuration() > (uint16_t)(longClickDuration + (longPressCounter * longPressInterval)) ) ) {
                    longPressCounter++;
                    if ( !dragEnabled && (repeatLongPress || longPressCounter == 1) ) {
                        invoke(InputEventType::LONG_PRESS);
                    }
                }
            }
            //Fire all the clicks etc
            if (!clickFired && !touched && currentDuration() > multiClickInterval) {
                clickFired = true;
                if (previousDuration() > longClickDuration || longPressCounter > 0 ) {
                    clickCounter = 0;
                    prevClickCount = 1;
                    invoke(InputEventType::LONG_CLICKED);
                    longPressCounter = 0;
                } else {
                    if ( clickCounter == 1 ) {
                        invoke(InputEventType::CLICKED);
                    } else if (clickCounter == 2 ) {
                        invoke(InputEventType::DOUBLE_CLICKED);
                    } else {
                        invoke(InputEventType::MULTI_CLICKED);
                    }
                    clickCounter = 0;
                }
            }
            EventInputBase::update();
        }
    }
}


void EventTouchScreen::invoke(InputEventType et) {
    if ( isInvokable(et) ) {
        callbackFunction(et, *this);
    }    
}

void EventTouchScreen::onDisabled() {
    //Reset button state
    clickCounter = 0;
    longPressCounter = 0;
    invoke(InputEventType::DISABLED);
}



bool EventTouchScreen::isPressed() { return false; } //buttonState() == LOW; }

uint16_t EventTouchScreen::currentDuration() {
    return millis() - lastStateChange;
}

void EventTouchScreen::changeState(bool t) {
    touched = t;
    prevDuration = currentDuration();
    lastStateChange = millis();
}


bool EventTouchScreen::haveDragged() {
    uint16_t dMs = dragging ? dragIntervalMs : dragThresholdMs;
    if ( dMs < millis() - lastDragMs ) {
        uint16_t dPx = dragging ? dragIntervalPx : dragThresholdPx;
        uint16_t dx = abs(touchPoint.x - startTouchPoint.x);
        uint16_t dy = abs(touchPoint.y - startTouchPoint.y);
        uint16_t distance = (dx * dx + dy * dy);  // Euclidean distance
        //Occasionally I do a bit of Pythagoras but I can give it up if I want to.
        if ( distance > (dPx*dPx) ) { //Avoid sqrt
            dragging = true;
            lastDragMs = millis();
            return true;
        }
    }
    return false;
}

bool EventTouchScreen::debounced() {
    //Don't report change if within bounce interval
    if ( millis() < lastBounceCheck+bounceInterval ) {
        return false;
    }
    //We have exceeded bounceInterval
    TouchPoint_s tp = touchAdapter->getTouchPoint();
    bool bounceState = (tp.z != 0);
    if ( previousBounceState != bounceState ) { //State has changed
        previousBounceState = bounceState;
        lastBounceCheck = millis();
        return false;
    }
    //State is the same and bounceInterval has passed
    lastBounceCheck = millis();
    touchPoint = tp;
    return true;
}   


} //namespace
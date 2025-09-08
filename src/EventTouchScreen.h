/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_EVENT_TOUCH_SCREEN_H
#define INPUT_EVENTS_EVENT_TOUCH_SCREEN_H

#include "Arduino.h"

#include "InputEvents.h"
#include "EventInputBase.h"
#include "TouchScreenAdapter/BaseTouchScreenAdapter.h"

namespace input_events {

/**
 * @brief The EventTouchScreen class enables a touch screen to return InputEvents EventButton events plus DRAG and DRAGGED.
 * 
 */
class EventTouchScreen : public EventInputBase {

    protected:

    #if defined(FUNCTIONAL_SUPPORTED)
    /**
     * @brief If <code>std::function</code> is supported, use to create the callback type.
     */
    typedef std::function<void(InputEventType et, EventTouchScreen &ts)> CallbackFunction;
    #else
    /**
     * @brief Create the callback type as pointer if <code>std::function</code> is not supported.
     */
    typedef void (*CallbackFunction)(InputEventType et, EventTouchScreen &);
    #endif

    /**
     * @brief The callback function member.
     */
    CallbackFunction callbackFunction = nullptr;


    public:

    /**
     * @brief Construct a new EventTouchScreen object
     * 
     * @param touchAdapter A previously created BaseTouchScreenAdapter passed by reference.
     */
    EventTouchScreen(ITouchScreenAdapter* touchAdapter);

    /**
     * @brief Initialise the EventTouchScreen
     * 
     * @details This will also call <code>begin()</code> for the passed TouchScreenAdapter.
     * 
     */
    void begin();

    /**
     * @brief Set the Callback function.
     * 
     * @param f A function of type <code>EventTouchScreen::CallbackFunction</code> type.
     */
    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    /**
     * @brief Set the Callback function to a class method.
     * 
     * @details Note: This method is only available if <code>std:function</code> is supported.
     * 
     * 
     * @param instance The instance of a class implementing a CallbackFunction method.
     * @param method The class method of type <code>EventTouchScreen::CallbackFunction</code> type.
     */
    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventTouchScreen&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventTouchScreen& ie) {
            (instance->*method)(et, ie); // Call the member function on the instance
        };
        callbackIsSet = true;
    }
    #endif

    /**
     * @brief Unset a previously set callback function or method.
     * 
     * @details Must be called if the previously set method is destroyed.
     * 
     */
    void unsetCallback() override;

    /**
     * @brief Update the state of the touch screen.
     * 
     * @details *Must* be called from within <code>loop()</code>
     * 
     */
    void update();

    /**
     * @brief Set the interval in ms between double, triple or
     * multi clicks
     */
    void setMultiClickInterval(unsigned int intervalMs=250) { multiClickInterval = intervalMs; }

    /**
     * @brief Set the ms that defines a long click. Long pressed callback
     * will be fired at this interval if repeat is set to true via the
     * setLongPressHandler()
     */
    void setLongClickDuration(unsigned int longDurationMs=750) { longClickDuration = longDurationMs; }

    /**
     * @brief Choose whether to repeat the long press callback (default is 'false')
     */
    void enableLongPressRepeat(bool repeat=true) { repeatLongPress = repeat; }

    /**
     * @brief Set the interval in ms between long press repeats
     * if enabled
     */
    void setLongPressInterval(unsigned int intervalMs=500) { longPressInterval = intervalMs; }

    /**
     * @brief The number of multi-clicks that have been fired in the clicked event
     */
    unsigned char clickCount() { return prevClickCount; }


    /**
     * @brief The number of times the long press handler has  been fired in the 
     * button pressed event
     */
    uint8_t longPressCount() { return longPressCounter; }

    /**
     * @brief Returns true if touch screen is pressed/touched
     */
    bool isPressed();

    /**
     * @brief Returns true if a drag event has occurred
     */
    bool isDragging() { return dragging; }

    /**
     * @brief Enable dragging. Dragging is disabled by default but
     * when enabled, LONG_PRESS event is not fired although long press count 
     * and LONG_CLICK are retained if no DRAGGED event occurs.
     * 
     * @param allow True (default) to enable, false to disable
     */
    void enableDragging(bool allow = true) { dragEnabled = allow; }

    /**
     * Returns true if dragging is enabled.
     */
    bool isDragEnabled() { return dragEnabled; }

    /**
     * @brief Set the pixel threshold before firing the *first* DRAGGED event 
     * Used in combination with setDragThresholdMs()
     * 
     * @param px A number of pixels
     */
    void setDragThresholdPx(uint8_t px ) { dragThresholdPx = px; }

    /**
     * @brief Set the pixel threshold before firing the *subsequent* DRAGGED event 
     * Used in combination with setDragIntervalMs()
     * 
     * @param px A number of pixels
     */
    void setDragIntervalPx(uint8_t px ) { dragIntervalPx = px; }

    /**
     * @brief Set the time threshold moved to fire the *first* DRAGGED event 
     * Used in combination with setDragThresholdMs()
     * 
     * @param ms 
     */
    void setDragThresholdMs(uint8_t ms ) { dragThresholdMs = ms; }

    /**
     * @brief Set the time threshold moved to fire the *subsequent* DRAGGED events 
     * Used in combination with setDragThresholdMs()
     * 
     * @param ms 
     */
    void setDragIntervalMs(uint8_t ms ) { dragIntervalMs = ms; }

    /**
     * @brief Set the rotation touch screen.
     * 
     * @details This should be called at the same time the display rotation is set.
     * 
     * @param r Follows the convention of:
     *  - 0 = native orientation
     *  - 1 = 90degrees from native
     *  - 2 = 180degrees from native
     *  - 3 = 270degrees from native
     */
    void setRotation(uint8_t r ) {
        touchAdapter->setRotation(r);
    }

    /**
     * @brief Set the Rate Limit for updates to the touch screen in milliseconds.
     * 
     * @details Use this to allow time for other tasks to be performed in your <code>loop()</code>
     * Default is 10ms
     * 
     * @param ms 
     */
    void setRateLimit(uint16_t ms) { rateLimit = ms; }

    /**
     * @brief Set the debounce interval in milliseconds.
     * 
     * @details Just like physical buttons, screens can 'bounce' as they are pressed or released.
     * This is particularly prevelant in resistive touch panels which are generally much slower to respond and require an 'assertive' touch.
     * 
     * The default bounce interval is 15ms but you will likely need to increase this for resistive panels. I have found 30-40ms works for mine.
     * 
     * @param ms 
     */
    void setDebounceInterval(uint16_t ms) { bounceInterval = ms; }


    /**
     * @brief The duration of the current pressed or released state.
     * 
     * @return uint16_t The duration in milliseconds.
     */
    uint16_t currentDuration();

    /**
     * @brief The duration of the previous pressed or released state.
     * 
     * @return uint16_t The duration in milliseconds.
     */
    uint16_t previousDuration() { return prevDuration; }


    /**
     * @brief Get the TouchPoint_s struct containing the touched X and Y display coordinates.
     * If Z is 0, use the last known touched point but set Z to 0 before returning X & Y
     * 
     * @return TouchPoint_s 
     */
    TouchPoint_s getTouchPoint() { 
        if ( touchPoint.z == 0 ) { 
            touchPoint = lastTouchedPoint;
            touchPoint.z = 0;
        }
        return touchPoint; 
    }
    /**
     * @brief Get the Previous TouchPoint_s struct
     * 
     * @return TouchPoint_s 
     */
    TouchPoint_s getPreviousTouchPoint() { return previousTouchPoint; }

    /**
     * @brief Get the starting TouchPoint_s struct
     * 
     * @details Used for DRAGGED or DRAGGED_RELEASED events. 
     * 
     * @return TouchPoint_s 
     */
    TouchPoint_s getStartTouchPoint() { return startTouchPoint; }

    /**
     * @brief Get the TouchAdapter for this screen
     * 
     * @return ITouchScreenAdapter& 
     */
    ITouchScreenAdapter& getTouchAdapter() {
        return *touchAdapter;
    }

    protected:

    /**
     * @brief Override of the <code>EventInputBase::invoke()</code> virtual method.
     * 
     * @param et Enum of type <code>InputEventType</code>
     */
    void invoke(InputEventType et) override;

    /**
     * @brief Override to reset clickCounter and longPressCounter
     * 
     */
    void onDisabled() override;

    /**
     * @brief Change the pressed/touched or released/untouched state.
     * 
     * @param t Pass true for pressed or false for released.
     */
    void changeState(bool t);

    /**
     * @brief Return true if have dragged
     * 
     * @return true If time+distance after PRESSED or time only after DRAGGED
     * @return false If not (yet) dragged
     */
    bool haveDragged();

    /**
     * @brief Return true if the touched/pressed (or untouched/released) state is stable.
     * 
     * @details The current touchPoint is updated only if the state is stable.
     * 
     * @return true If touched state is stable
     * @return false If touched state is false
     */
    bool debounced();



private:

    ITouchScreenAdapter* touchAdapter = nullptr;

    //state
    bool previousBounceState = false;
    uint32_t lastBounceCheck = 0;
    uint16_t bounceInterval = 15; //Ms

    bool touched = false;
    uint8_t clickCounter = 0;
    uint8_t prevClickCount = 0;
    bool clickFired = true;

    bool dragging = false;
    uint32_t lastDragMs = 0;


    uint32_t lastStateChange = 0;
    uint16_t duration = 0;
    uint16_t prevDuration = 0;

    TouchPoint_s touchPoint = TouchPoint_s();
    TouchPoint_s startTouchPoint = TouchPoint_s();
    TouchPoint_s previousTouchPoint = TouchPoint_s();
    TouchPoint_s lastTouchedPoint = TouchPoint_s();
    

    //setup
    uint16_t multiClickInterval = 300; //250;
    uint16_t longClickDuration = 750;
    bool repeatLongPress = true;
    uint16_t longPressInterval = 500;
    uint16_t longPressCounter = 0;

    uint16_t rateLimit = 10;
    uint32_t rateLimitCounter = 0;

    bool dragEnabled = false;
    uint16_t dragThresholdPx = 20;
    uint16_t dragIntervalPx = 10;
    uint16_t dragThresholdMs = 200;
    uint16_t dragIntervalMs = 100;
    uint16_t postDragRateLimit = 500;

};

}
#endif

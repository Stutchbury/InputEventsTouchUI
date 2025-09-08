/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_BASE_TOUCH_KEYPAD_WIDGET_H
#define INPUT_EVENTS_BASE_TOUCH_KEYPAD_WIDGET_H

#ifndef TOUCH_KEYPAD_MAX_ROWS
#define TOUCH_KEYPAD_MAX_ROWS 4
#endif
#ifndef TOUCH_KEYPAD_MAX_COLS
#define TOUCH_KEYPAD_MAX_COLS 5
#endif

#include "Arduino.h"

#include "ui/BaseWidget.h"
#include "ui/TouchWidgetMixin.h"
#include "TouchKeypadKey.h"

namespace input_events {

/**
 * @brief A base class for creating touch keypads. This class organises TouchKeypadKey's into a grid format. As with other widgets, drawing is delegated to the concrete class.
 * 
 */
template<size_t NumRows, size_t NumCols> 
class BaseTouchKeypadWidget : public BaseWidget, 
                            public TouchWidgetMixin<BaseTouchKeypadWidget<NumRows, NumCols>> 
                            {

    public:

    /**
     * @brief Construct a BaseTouchKeypadWidget with a Region
     * 
     * @param region 
     */
    BaseTouchKeypadWidget(Region region) 
        : BaseWidget(region) //,
        {
            initKeys();
        }

    /**
     * @brief Construct a BaseTouchKeypadWidget with x, y, width and height
     * 
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     */
    BaseTouchKeypadWidget(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
        : BaseTouchKeypadWidget(Region(x, y, w, h))
        {}

    /**
     * @brief Enable all TouchKeypadKey's
     * 
     */
    void begin() override {
        for ( uint8_t r = 0; r < NumRows; r++ ) {
            for ( uint8_t c = 0; c < NumCols; c++ ) {
                touchKey[r][c].setState(WidgetDisplayState::ENABLED);
            }
        }
    }

    /**
     * @brief Called by a screen on start
     * 
     */
    void start() override {
        for ( uint8_t r = 0; r < NumRows; r++ ) {
            for ( uint8_t c = 0; c < NumCols; c++ ) {
                touchKey[r][c].start();
            }
        }
        redrawRequired();
    }

    /**
     * @brief Called to draw all TouchKeypadKey's
     * 
     */
    void draw() override {
        for ( uint8_t r = 0; r < NumRows; r++ ) {
            for ( uint8_t c = 0; c < NumCols; c++ ) {
                if ( !keyRemoved[r][c] && touchKey[r][c].isRedrawRequired() ) {
                    this->drawKey(touchKey[r][c]);
                    touchKey[r][c].redrawRequired(false);
                }
            }
        }
        redrawRequired(false);
    }


    /**
     * @brief Does nothing
     * 
     */
    void end() override {

    }

    /**
     * @brief Does nothing
     * 
     */
    void onStateChanged() override {

    }

    /**
     * @brief Callback to concrete class to draw a key - this will only be called if the key's `isRedrawRequired()` is true
     * 
     * @param key 
     */
    virtual void drawKey(TouchKeypadKey& key) = 0;

    /**
     * @brief Abstract method to be implemented by concrete classes
     * 
     * @param key 
     * @param et 
     * @param touchPanel 
     * @return true 
     * @return false 
     */
    virtual bool onTouchKeyEvent(TouchKeypadKey& key, InputEventType et, EventTouchScreen& touchPanel) = 0;

    //For TouchWidgetMixin
    /**
     * @brief For TouchWidgetMixin, calls each key and sets PRESSED or RELEASED state
     * 
     * @param et 
     * @param touchPanel 
     * @return true 
     * @return false 
     */
    bool onTouchEvent(InputEventType et, EventTouchScreen& touchPanel) override {
        if ( !contains(touchPanel.getStartTouchPoint())) return false; //Not in the keypad region
 
        for ( uint8_t r = 0; r < NumRows; r++ ) {
            for ( uint8_t c = 0; c < NumCols; c++ ) {
                if ( !keyRemoved[r][c] ) {
                    if ( et == InputEventType::PRESSED ) {
                        if ( touchKey[r][c].contains(touchPanel.getStartTouchPoint()) ) {
                            touchKey[r][c].setState(WidgetDisplayState::PRESSED);
                        }
                    } /*else*/ 
                    if ( et == InputEventType::RELEASED ) {
                        if ( touchKey[r][c].getState() == WidgetDisplayState::PRESSED) {
                            touchKey[r][c].setState(touchKey[r][c].getPreviousState()); //Release any key that is pressed
                        }
                    }
                    if ( touchKey[r][c].contains(touchPanel.getStartTouchPoint() ) ) {
                        return onTouchKeyEvent(touchKey[r][c], et, touchPanel);
                    }
                }
            }
        }
        return false;
    }

    /**
     * @brief Get a TouchKeypadKey for a row and column
     * 
     * @param row 
     * @param col 
     * @return TouchKeypadKey* or nullptr if the key has been removed.
     */
    TouchKeypadKey* getKey(uint8_t row, uint8_t col ) {
        if ( row >= NumRows || col >= NumCols ) return nullptr;
        if ( keyRemoved[row][col] ) return nullptr;
        return &touchKey[row][col];
    }

    /**
     * @brief Return the Region of a key (even if it is removed). Will return nam empty region if row or col is out of bounds
     * 
     * @param row 
     * @param col 
     * @return Region 
     */
    Region getKeyRegion(uint8_t row, uint8_t col ) {
        if ( row >= NumRows || col >= NumCols ) return Region();
        return Region(touchKey[row][col]);
    }

    /**
     * @brief Mark a key as removed. Removing a key means it will never be passed by drawKey, onTouchKeyEvent etc or returned by getKey()
     * 
     * @param row 
     * @param col 
     * @param remove Default true - pass false to 'un-remove'
     */
    void removeKey(uint8_t row, uint8_t col, bool remove=true) {
        if ( row >= NumRows || col >= NumCols ) return;
        keyRemoved[row][col] = remove;
    }


    private:

    void initKeys() {
        for ( uint8_t r = 0; r < NumRows; r++ ) {
            for ( uint8_t c = 0; c < NumCols; c++ ) {
                touchKey[r][c] = TouchKeypadKey(xDiv(NumCols, c), yDiv(NumRows, r), wDiv(NumCols), hDiv(NumRows), r, c);
                keyRemoved[r][c] = false;
            }
        }
    }

    TouchKeypadKey touchKey[NumRows][NumCols];
    bool keyRemoved[NumRows][NumCols];


};
} //nanespace
#endif
/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_TOUCH_KEYPAD_KEY_H
#define INPUT_EVENTS_TOUCH_KEYPAD_KEY_H

#include "Arduino.h"


//#include "TouchKeypad/BaseTouchKeypadWidget.h"
#include "ui/BaseWidget.h"
#include "ui/TouchWidgetMixin.h"

namespace input_events {

/**
 * @brief Key class for the BaseTouchKeypadWidget class.
 * 
 */
class TouchKeypadKey : public BaseWidget {

    public:

    TouchKeypadKey() : BaseWidget(0,0,1,1) {} //Because C++ needs a default constructor :-/

    /**
     * @brief Construct a key at x, y with width and height and specified row & col in the keypad
     * 
     */
    TouchKeypadKey(uint16_t x, uint16_t y, uint16_t w, uint16_t h , uint8_t row, uint8_t col ) //, BaseTouchKeypadWidget* keypad ) 
    : BaseWidget(x, y, w, h),
      keyRow(row),
      keyCol(col)
    {}


    /**
     * @brief Called when the screen becomes current
     * 
     */
    void start() override {
        redrawRequired();
    }

    /**
     * @brief Does nothing
     * 
     */
    void draw() override {}


    /**
     * @brief Does nothing
     * 
     */
    void clear() override {}

    /**
     * @brief Does nothing
     * 
     */
    void end() override {} //Handled by BaseTouchKeypadWidget

    /**
     * @brief Set redrawRequired when state changes
     * 
     */
    void onStateChanged() override {
        redrawRequired();
    }

    /**
     * @brief The row position of this key
     * 
     * @return uint8_t 
     */
    uint8_t row() { return keyRow; }
    /**
     * @brief The column position of this key
     * 
     * @return uint8_t 
     */
    uint8_t col() { return keyCol; }

    /**
     * @brief Equality operator
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator==(const TouchKeypadKey& other) const {
        return keyRow == other.keyRow && keyCol == other.keyCol; //????These are private!
    }

    /**
     * @brief Equality operator
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    bool operator!=(const TouchKeypadKey& other) const {
        return !(*this == other);
    }

    protected:

    private:
    uint8_t keyRow = 0;
    uint8_t keyCol = 0;


};
} //namespace
#endif
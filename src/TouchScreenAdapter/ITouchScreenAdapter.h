/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */


#ifndef INPUT_EVENTS_ITOUCH_SCREEN_ADAPTER_H
#define INPUT_EVENTS_ITOUCH_SCREEN_ADAPTER_H

#include <Arduino.h>
//#include "Coords_s.h"
#include "TouchPoint_s.h"

namespace input_events {

/**
 * @brief A lightweight abstract Abstract class for touch screen panels.
 * 
 */
class ITouchScreenAdapter {

public:

    /**
     * @brief Set the native (non-rotated) display width (X) in pixels
     * 
     * @details Is set to 240 by default. For some adapters (resistive) this *must* be set to the native width of your display.
     * For adapters that report pixel position directly, this is only reqired if X and/or Y need to be reversed. (Ahem, FT6206).
     * 
     * @param widthPx Width in pixels
     */
    virtual void setDisplayWidth(uint16_t widthPx) = 0;

    /**
     * @brief Set the native (non-rotated) display height (Y) in pixels
     * 
     * @details Is set to 320 by default. For some adapters (resistive) this *must* be set to the native height of your display.
     * For adapters that report pixel position directly, this is only used to reverse X or Y.
     * 
     * @param heightPx Width in pixels
     */
    virtual void setDisplayHeight(uint16_t heightPx) = 0;

    /**
     * @brief Get the TouchPoint_s object 
     * 
     * @details Returns a TouchPoint_s. Display are x & y populated (ie non-zero) only if touched.
     * 
     * @return TouchPoint_s 
     */
    virtual TouchPoint_s getTouchPoint(void) = 0;

    /**
     * @brief Get a TouchPoint_s struct containing raw values from the underlying library. 
     * 
     * @details Returns a TouchPoint_s with raw x, y & z populated from the underlying library. For most panels this will usually return the same values as getTouchPoint() but can be useful for tuning resistive panels.
     * Note: This will return a populated TouchPoint_s irrespective of the Z value. Generally used for setup or debugging.
     * 
     * @return TouchPoint_s 
     */
    virtual TouchPoint_s getTouchPointRaw(void) = 0;

    /**
     * @brief For compatibility with the Arduino library convention.
     * 
     * @details Must be implemented (but can be empty) as some touch libraries require it. Can return a boolean to idicate if initialisation has failed.
     * 
     * @return true The initialisation was successful.
     * @return false The initialisation was failed.
     */
    virtual bool begin(void) = 0;

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
    virtual void setRotation(uint8_t r ) = 0;

};
} //namespace
#endif
/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */


#ifndef INPUT_EVENTS_BASE_TOUCH_SCREEN_ADAPTER_H
#define INPUT_EVENTS_BASE_TOUCH_SCREEN_ADAPTER_H

#include <Arduino.h>
#include "ITouchScreenAdapter.h"
//#include "Coords_s.h"

//#include "TouchPoint_s.h"

namespace input_events {

/**
 * @brief A lightweight abstract Abstract class for touch screen panels.
 * 
 */
class BaseTouchScreenAdapter : public ITouchScreenAdapter { 

public:

    /**
     * @brief Set the native (non-rotated) display width (X) in pixels
     * 
     * @details Is set to 240 by default. For some adapters (resistive) this *must* be set to the native width of your display.
     * For adapters that report pixel position directly, this is only reqired if X and/or Y need to be reversed. (Ahem, FT6206).
     * 
     * @param widthPx Width in pixels
     */
    void setDisplayWidth(uint16_t widthPx) override { 
        nativeDisplayWidth = widthPx; 
        displayWidth = nativeDisplayWidth;
        setRotation(rotation);
    }

    /**
     * @brief Set the native (non-rotated) display height (Y) in pixels
     * 
     * @details Is set to 320 by default. For some adapters (resistive) this *must* be set to the native height of your display.
     * For adapters that report pixel position directly, this is only used to reverse X or Y.
     * 
     * @param heightPx Width in pixels
     */
    void setDisplayHeight(uint16_t heightPx) override {
        nativeDisplayHeight = heightPx; 
        displayHeight = nativeDisplayHeight;
        setRotation(rotation);
    }

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
        rotation = (r & 3);
        switch (rotation) {
        case 0:
        case 2:
            displayWidth = nativeDisplayWidth;
            displayHeight = nativeDisplayHeight;
            break;
        case 1:
        case 3:
            displayWidth = nativeDisplayHeight;
            displayHeight = nativeDisplayWidth;
            break;
        }
    }

protected:

    /**
     * @brief The current rotation of the touch panel. Default is 0 (native orientation).
     * 
     */
    uint8_t rotation = 0;

    uint16_t nativeDisplayWidth = 240; ///< The non-rotated width of the display in pixels.
    uint16_t nativeDisplayHeight = 320; ///< The non-rotated height of the display in pixels.

    uint16_t displayWidth = nativeDisplayWidth; ///< The (optionally rotated) width of the display in pixels.
    uint16_t displayHeight = nativeDisplayHeight; ///< The (optionally rotated) height of the display in pixels.

};
} //namespace
#endif
/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_TOUCH_POINT_S_H
#define INPUT_EVENTS_TOUCH_POINT_S_H
#include <Arduino.h>
#include "Coords_s.h"

namespace input_events {

/**
 * @brief A minimal struct that represents the Coords_s on a display plus z to indicate touch (either boolen or pressure). z==0 means not touched.
 * 
 */
struct TouchPoint_s : public Coords_s {
    uint16_t z = 0;  ///< Denotes if screen is touched. Can also show pressure if adapter supports it

    constexpr TouchPoint_s() = default;

    /**
     * @brief Construct a TouchPoint_s struct passing x, y & z
     * 
     */
    constexpr TouchPoint_s(uint16_t _x, uint16_t _y, uint16_t _z = 0)
        : Coords_s(_x, _y), z(_z) {}

    /**
     * @brief equality operator
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    constexpr bool operator==(const TouchPoint_s& other) const {
        return Coords_s::operator==(other) && z == other.z;
    }
    /**
     * @brief equality operator that omits z comparison
     * 
     * @param coords 
     * @return true 
     * @return false 
     */
    bool operator==(const Coords_s& coords) const {
        return Coords_s::operator==(coords);
    }        
};

/**
 * @brief Global equality operator
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
inline bool operator==(const Coords_s& lhs, const TouchPoint_s& rhs) {
    return rhs == lhs; // Reuse member operator
}

} //namespace
#endif
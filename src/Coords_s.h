/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_COORDS_S_H
#define INPUT_EVENTS_COORDS_S_H
#include <Arduino.h>

namespace input_events {

/**
 * @brief A minimal struct that represents the x and y coordinates on a display.
 * 
 */
struct Coords_s {
    uint16_t x; ///< the x position on the display
    uint16_t y; ///< the y position on the display

    /**
     * @brief Constructor of a Coords_s pair
     * 
     */
    constexpr Coords_s(uint16_t x = 0, uint16_t y = 0) : x(x), y(y) {}

    /**
     * @brief equality operator
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    constexpr bool operator==(const Coords_s& other) const {
        return x == other.x && y == other.y;
    }  
};

/**
 * @brief Syntactic sugar for static_cast<uint16_t>
 * 
 * @param v 
 * @return uint16_t 
 */
inline uint16_t u16(int v) { return static_cast<uint16_t>(v); }


} //namespace
#endif
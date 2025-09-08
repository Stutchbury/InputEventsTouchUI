/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_REGION_H
#define INPUT_EVENTS_REGION_H

//#include <cassert>
#include <Arduino.h>
#include "Coords_s.h"

namespace input_events {

/**
 * @brief A Region defines an area of the display and provides a host
 * of utility methods for determining the absolute display x, y
 * posion of various attributes such as b() (bottom), l() (left),
 * cp() (centre point) etc.
 * 
 * @details Method names are deliberately kept short (concise, or terse, even)
 * because they are frequently called within the arguments of other
 * class methods.
 */
class Region {
    protected:
    uint16_t _x = 0; ///< The leftmost x positions of the Region
    uint16_t _y = 0; ///< The top y positions of the Region
    uint16_t _w = 0; ///< The width of the Region
    uint16_t _h = 0; ///< The height of the Region

    // saturating add for uint16_t
    /**
     * @brief Clamp addition so a padded Region's x and y cannot become 'interesting' numbers.
     * 
     * @param base 
     * @param delta 
     * @return uint16_t 
     */
    inline uint16_t clamp_add(uint16_t base, int delta) {
        int temp = static_cast<int>(base) + delta;
        if (temp < 0) return 0;
        if (temp > 65535) return 65535;
        return static_cast<uint16_t>(temp);
    }

    // saturating subtract for uint16_t
    /**
     * @brief Clamp subtraction so a padded Region's w and h cannot become 'interesting' numbers.
     * 
     * @param base 
     * @param delta 
     * @return uint16_t 
     */
    inline uint16_t clamp_minus(uint16_t base, int delta) {
        return clamp_add(base, -delta);
    }

    public:
    /**
     * @brief Default constructor
     */
    constexpr Region() = default;


    ///@{ 
    /** 
     * @name Constructors
     */
    /**
     * @brief Construct a Region from x, y coordinates and width & height
     * 
     * @details Will silently set w or h = 1 of they are passed as 0.
     */
    // constexpr Region(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    //     : _x(x), _y(y), _w(w), _h(h) 
    //     {
    //         if (w == 0 ) _w = 1;
    //         if (h == 0 ) _h = 1;
    //     }
    constexpr Region(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
        : _x(x), _y(y), _w(w ? w : 1), _h(h ? h : 1) {}

    /**
     * @brief Construct a Region from a Coords_s struct and width & height
     * 
     */
    constexpr Region(Coords_s coords, uint16_t w, uint16_t h)
        : Region(coords.x, coords.y, w, h) {}

    /**
     * @brief Construct a Region from another Region
     * 
     */
    constexpr Region(const Region& region)
        : Region(region._x, region._y, region._w, region._h) {}

    ///@}

    ///@{ 
    /** 
     * @name Setters
     */

    /**
     * @brief Set a new x position
     */
    void setX(uint16_t x) { _x = x; }
    /**
     * @brief Set a new y position
     */
    void setY(uint16_t y) { _y = y; }
    /**
     * @brief Set a new width. Width cannot be 0.
     */
    void setW(uint16_t w) { 
        if ( w == 0 ) return; 
        _w = w; 
    }
    /**
     * @brief Set a new height. Height cannot be 0.
     */
    void setH(uint16_t h) { 
        if ( h== 0 ) return;
        _h = h; 
    }


    /**
     * @brief Set the position and dimenensions to that of another region.
     * 
     * @param region 
     */
    void setRegion(const Region& region) {
        _x = region._x;
        _y = region._y;
        _w = region._w;
        _h = region._h;
    }

    /**
     * @brief Pad a Region by the same amount or percentage on all sides
     * 
     * @param all Number of pixels to pad or if less the 1, the percentage to pad.
     */
    void pad(float all) {
        pad(all, all);
    }

    /**
     * @brief Pad a Region using different values for width and height
     * 
     * @param vertical Number of pixels to pad vertically or if less the 1, the percentage to pad.
     * @param horizontal Number of pixels to pad horizontally or if less the 1, the percentage to pad.
     */
    void pad(float vertical, float horizontal) {
        pad(vertical, horizontal, vertical, horizontal);
    }

    /**
     * @brief Pad a Region. Follows similar convention to CSS. If values are less then 1 will be treated as a percentage.
     * 
     * @details The Region's x, y, w, & h cannot be negative but it is possible to create a zero width/height region! Passing negative values will make a region larger.
     * 
     * @param top 
     * @param right 
     * @param bottom 
     * @param left 
     */
    void pad(float top, float right, float bottom, float left) {
        int16_t _top = static_cast<int16_t>( (top < 1.0f && top > -1.0f) ? h()*top : top );
        int16_t _right = static_cast<int16_t>( (right < 1.0f && right > -1.0f) ? w()*right : right );
        int16_t _bottom = static_cast<int16_t>( (bottom < 1.0f && bottom > -1.0f) ? h()*bottom : bottom );
        int16_t _left = static_cast<int16_t>( (left < 1.0f && left > -1.0f) ? w()*left : left);
        _x = clamp_add( _x, _left);
        _y = clamp_add(_y, _top);
        _w = clamp_minus(_w, (_left + _right));
        _h = clamp_minus(_h, (_top + _bottom));
    }

    ///@}

    ///@{ 
    /** 
     * @name Get position and dimensions
     */

    /**
     * @brief Returns the absolute x position
     */
    constexpr uint16_t x() const { return _x; }
    /**
     * @brief Returns the absolute y position
     */
    constexpr uint16_t y() const { return _y; }
    /**
     * @brief Returns the width
     */
    constexpr uint16_t w() const { return _w; }
    /**
     * @brief Returns the height
     */
    constexpr uint16_t h() const { return _h; }

    /**
     * @brief Returns right-most x absolute position
     */
    constexpr uint16_t r() const { return _x + _w - 1; }
    /**
     * @brief Returns bottom y absolute position
     */
    constexpr uint16_t b() const { return _y + _h - 1; }

    ///@}

    ///@{ 
    /** 
     * @name Calculated dimensions and positions
     */
    /**
     * @brief Returns width divided by div (default 2)
     */
    constexpr uint16_t wDiv(uint8_t div = 2) const {
        return _w / (div == 0 ? 1 : div);
    }

    /**
     * @brief Returns height divided by div (default 2)
     */
    constexpr uint16_t hDiv(uint8_t div = 2) const {
        return _h / (div == 0 ? 1 : div);
    }

    /**
     * @brief Returns nth absolute X position of width divided by div
     */
    constexpr uint16_t xDiv(uint8_t div = 2, uint8_t nth = 0) const {
        return _x + nth * wDiv(div);
    }

    /**
     * @brief Returns nth absolute y position of height divided by div
     */
    constexpr uint16_t yDiv(uint8_t div = 2, uint8_t nth = 0) const {
        return _y + nth * hDiv(div);
    }

    /**
     * @brief Returns nth absolute right position of width divided by div
     */
    constexpr uint16_t rDiv(uint8_t div = 2, uint8_t nth = 0) const {
        return xDiv(div, nth) + wDiv(div);
    }

    /**
     * @brief Returns nth absolute Y position of bottom with height divided by div
     */
    constexpr uint16_t bDiv(uint8_t div = 2, uint8_t nth = 0) const {
        return yDiv(div, nth) + hDiv(div);
    }

    // Centre lines
    /**
     * @brief Returns absolute X position of centre line with height divided by div
     */
    constexpr uint16_t xCl(uint8_t div = 1, uint8_t nth = 0) const {
        return xDiv(div, nth) + wDiv(div) / 2;
    }

    /**
     * @brief Returns absolute Y position of centre line with height divided by div
     */
    constexpr uint16_t yCl(uint8_t div = 1, uint8_t nth = 0) const {
        return yDiv(div, nth) + hDiv(div) / 2;
    }

    /**
     * @brief Returns absolute centre point as Coords_s
     */
    constexpr Coords_s cp() const { return { xCl(), yCl() }; }
    ///@}

    ///@{ 
    /** 
     * @name Corners
     */

    // Corner positions (requires Coords_s to be constexpr-constructible)
    /**
     * @brief Returns absolute top left as Coords_s
     */
    constexpr Coords_s tl() const { return { _x, _y }; }
    /**
     * @brief Returns absolute top right as Coords_s
     */
    constexpr Coords_s tr() const { return { r(), _y }; }
    /**
     * @brief Returns absolute bottom right as Coords_s
     */
    constexpr Coords_s br() const { return { r(), b() }; }
    /**
     * @brief Returns absolute bottom left as Coords_s
     */
    constexpr Coords_s bl() const { return { _x, b() }; }
    ///@}

    ///@{ 
    /** 
     * @name Containment and intersection
     */

    /**
     * @brief Returns true if region contains posX and posY
     */
    constexpr bool contains(uint16_t posX, uint16_t posY) const {
        return _x <= posX && posX <= r() && _y <= posY && posY <= b();
    }

    /**
     * @brief Returns true if region contains Coord_s
     */
    constexpr bool contains(const Coords_s& coords) const {
        return contains(coords.x, coords.y);
    }

    /**
     * @brief Returns true if Region contains Region 
     */
    constexpr bool contains(const Region& r) const {
        return contains(r._x, r._y) && contains(r.r(), r.b());
    }

// constexpr bool intersects(const Region& r, bool inclusive = true) const {
//     if (inclusive) {
//         return !(
//             r.r() < _x ||
//             r._x > this->r() ||
//             r.b() < _y ||
//             r._y > this->b()
//         );
//     } else {
//         return !(
//             r.r() <= _x - 1 ||
//             r._x >= this->r() + 1 ||
//             r.b() <= _y - 1 ||
//             r._y >= this->b() + 1
//         );
//     }
// }
// constexpr bool intersects(const Region& other, bool inclusive = true) const {
//     uint16_t leftA = _x, rightA = _x + _w;
//     uint16_t topA = _y, bottomA = _y + _h;
//     uint16_t leftB = other._x, rightB = other._x + other._w;
//     uint16_t topB = other._y, bottomB = other._y + other._h;

//     if (inclusive) {
//         return !(rightB <= leftA || leftB >= rightA || bottomB <= topA || topB >= bottomA);
//     } else {
//         return !(rightB <= leftA || leftB >= rightA || bottomB <= topA || topB >= bottomA);
//     }
// }

// constexpr bool intersects(const Region& other, bool inclusive = true) const {
//     if (inclusive) {
//         return !(other.r() < _x || other.x() > r() || other.b() < _y || other.y() > b());
//     } else {
//         return !(other.r() <= _x || other.x() >= r() || other.b() <= _y || other.y() >= b());
//     }
// }

/**
 * @brief Does this Region intersect with another Region (needs a bit more work!)
 * 
 * @param other 
 * @param inclusive 
 * @return true 
 * @return false 
 */
constexpr bool intersects(const Region& other, bool inclusive = true) const {
    return inclusive
        ? !(other.r() < _x || other.x() > r() || other.b() < _y || other.y() > b())
        : !(other.r() <= _x || other.x() >= r() || other.b() <= _y || other.y() >= b());
}


    // constexpr bool intersects(const Region& r, bool inclusive = true) const {
    //     if (inclusive) {
    //         return !(
    //             r.r() < _x ||
    //             r._x > this->r() ||
    //             r.b() < _y ||
    //             r._y > this->b()
    //         );
    //     } else {
    //         return !(
    //             r.r() <= _x ||
    //             r._x >= this->r() ||
    //             r.b() <= _y ||
    //             r._y >= this->b()
    //         );
    //     }
    // }
    ///@}

};

}
#endif
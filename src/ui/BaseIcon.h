#ifndef INPUT_EVENTS_BASE_ICON_H
#define INPUT_EVENTS_BASE_ICON_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Region.h"

namespace input_events {


/**
 * @brief Parameters that can be passed to an icon to alter its drawn state.
 * 
 */
struct IconParams {
    uint16_t fg=0; ///< The foreground colour
    uint16_t bg=0; ///< The background colour
    uint16_t alt=0; ///< An alternate colour
    bool enabled=true; ///< If false, icon colours will bew greyscaled
    float scale=1;  ///< For future use
    uint16_t radius=0;  ///< the radius od a curcular icon - defaults to h()/2
    float pad=0; ///< 0 = no pad, >1 = pad, 0 to <1 = pad percent

    /**
     * @brief Construct a IconParams
     * 
     * @param fg Foreground colour
     * @param bg Background colour
     * @param enabled Enabled flag
     * @param scale
     * @param radius 
     * @param pad 
     */
    IconParams(uint16_t fg=0xFFFF, uint16_t bg=0x0000,
               bool enabled=true, float scale=1.0f, uint16_t radius=0, float pad=0)
        : fg(fg), bg(bg), enabled(enabled), scale(scale), radius(radius), pad(pad) {}

    /**
     * @brief Returns a copy of params with passed enabled flag
     * 
     * @param enabled_ 
     * @return IconParams 
     */
    IconParams withEnabled(bool enabled_) const {
        IconParams copy = *this;
        copy.enabled = enabled_;
        return copy;
    }

    /**
     * @brief Returns a copy of params with passed foreground colour
     * 
     * @param fg_ 
     * @return IconParams 
     */
    IconParams withFg(uint16_t fg_) const {
        IconParams copy = *this;
        copy.fg = fg_;
        return copy;
    }

    /**
     * @brief Returns a copy of params with passed background colour
     * 
     * @param bg_ 
     * @return IconParams 
     */
    IconParams withBg(uint16_t bg_) const {
        IconParams copy = *this;
        copy.bg = bg_;
        return copy;
    }

    /**
     * @brief Returns a copy of params with passed alternate colour
     * 
     * @param alt_ 
     * @return IconParams 
     */
    IconParams withAlt(uint16_t alt_) const {
        IconParams copy = *this;
        copy.alt = alt_;
        return copy;
    }

    /**
     * @brief Returns a copy of params with passed radius
     * 
     * @param rad_ 
     * @return IconParams 
     */
    IconParams withRadius(uint16_t rad_) const {
        IconParams copy = *this;
        copy.radius = rad_;
        return copy;
    }

    /**
     * @brief Returns a copy of params with passed padding
     * 
     * @param pad_ 
     * @return IconParams 
     */
    IconParams withPad(float pad_) const {
        IconParams copy = *this;
        copy.pad = pad_;
        return copy;
    }

    /**
     * @brief Convert a colour to greyscale
     * 
     * @param color 
     * @return uint16_t 
     */
    uint16_t disabledGrey(uint16_t color) {
        // Extract RGB components
        uint8_t r5 = (color >> 11) & 0x1F;
        uint8_t g6 = (color >> 5) & 0x3F;
        uint8_t b5 =  color        & 0x1F;

        // Convert to 8-bit
        uint8_t r = (r5 * 527 + 23) >> 6;
        uint8_t g = (g6 * 259 + 33) >> 6;
        uint8_t b = (b5 * 527 + 23) >> 6;

        // Weighted luma
        uint8_t grey = (77 * r + 150 * g + 29 * b) >> 8;

        // Remap from 0–255 to 64–192
        grey = (grey >> 1) + 64; // Equivalent to (gray * 0.5) + 64
        //grey = 128 + (grey - 128) * 90 / 100; // white a bit darker, black a bit lighter
        grey = 128 + (grey - 128) * 70 / 120; // white a bit darker, black a bit lighter
        // Back to 5/6/5
        return ((grey >> 3) << 11) | ((grey >> 2) << 5) | (grey >> 3);
    }

// uint16_t disabledGrey2(uint16_t color) {
//     // Extract RGB from 565
//     uint8_t r = (color >> 11) & 0x1F;
//     uint8_t g = (color >> 5)  & 0x3F;
//     uint8_t b =  color        & 0x1F;

//     // Scale to 8-bit per channel
//     r = (r * 255) / 31;
//     g = (g * 255) / 63;
//     b = (b * 255) / 31;

//     // Luma-weighted grayscale (better than straight average)
//     uint8_t gray = static_cast<uint8_t>((r * 30 + g * 59 + b * 11) / 100);

//     // Tune white & black levels
//     const float midpoint = 108.17f; // neutral gray
//     const float scale    = 0.353f;  // contrast pull
//     float adjusted = midpoint + (gray - midpoint) * scale;

//     // Clamp and convert back to 8-bit int
//     if (adjusted < 0) adjusted = 0;
//     if (adjusted > 255) adjusted = 255;
//     gray = static_cast<uint8_t>(adjusted);

//     // Convert back to RGB565
//     uint8_t r5 = gray >> 3;
//     uint8_t g6 = gray >> 2;
//     uint8_t b5 = gray >> 3;
//     return (r5 << 11) | (g6 << 5) | b5;
// }

    /**
     * @brief Convert colours to greyscale
     * 
     */
    void toGreyscale() {
        fg = disabledGrey(fg);
        bg = disabledGrey(bg);
        alt = disabledGrey(alt);
    }

};




/**
 * @brief 
 * @brief An Icon is stateless
 * 
 */
//template <typename Gfx>
class  BaseIcon {

public:
    /**
     * @brief 
     * 
     * @param region A copy of the Region (my be modified by the params.pad )
     * @param params The parameters to be used to draw this icon
     */
    void draw(Region region, IconParams params ) const {
        if ( !params.enabled ) params.toGreyscale();
        if ( params.pad != 0 ) region.pad(params.pad);
        if ( params.radius == 0 ) params.radius = static_cast<uint16_t>(min(region.h(), region.w())/2);
        drawIcon(region, params);
    }

    /**
     * @brief Abstract method to be implemented in the concrete classes (that actually draw the icon). Normally called via draw(), not directly.
     * 
     * @param region A copy of the Region (may be modified by the params.pad )
     * @param params The parameters to be used to draw this icon
     */
    virtual void drawIcon(Region region, IconParams params ) const = 0;

    virtual ~BaseIcon() {}

protected:

    /**
     * @brief Convert degrees to radians
     * 
     * @param a 
     * @return double 
     */
    double inline degree2radian2(int16_t a) const {
        return (a * DEG_TO_RAD);
    }

    /**
     * @brief Return only an even number - useful for diameter to radius etc.
     * 
     * @param n 
     * @return int 
     */
    int makeEven(int n) const { //rounds to nearest even number
        return (n % 2 == 0) ? n : (n + (n > 0 ? 1 : -1));
    }

};


} // namespace
#endif
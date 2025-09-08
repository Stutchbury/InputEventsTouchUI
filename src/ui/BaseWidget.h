/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_BASE_WIDGET_H
#define INPUT_EVENTS_BASE_WIDGET_H

//#include <optional>
#include <Arduino.h>
#include "Region.h"

namespace input_events {


 /**
  * @brief A list of display states for widgets
  * 
  */
enum class WidgetDisplayState : uint8_t {
    NONE,       ///< Not yet set or 'init'
    ENABLED,    ///< Change icon and/or colour 
    PRESSED,    ///< Change outline or background
    FOCUSSED,   ///< Change outline or background
    IDLE,       ///< Change outline or background
    DISABLED,   ///< Draw greyscaled but should not react to touch
    HIDDEN,     ///< Should not be drawn
    ERROR       ///< Widget is in error state 
};

/**
 * @brief Base class for TouchScreen widgets
 * 
 * @details Widgets are the 'building blocks' of an app's display. The hold a little bit of state and decide if (re)drawing is required.
 * They all draw to the screen but can be supplimented via Mixins to be use for touch, connected to physical buttons, or 'callback' a function or method.
 * 
 * Concrete widgets can have specialised setters and getters to set and optionally retrieve displayed state but will noramlly fetch state when draw() is called.
 * 
 * Some concrete widget classes will be a fixed width and height (until we do variable screen resolution) so their
 * constructors will likely accept X and Y (Coords_s or 2x uint16_t) rather than width and height.
 * 
 */
class BaseWidget : public Region {


    public:

    static const uint16_t WIDGET_COLOUR_DEFAULT_FG = 0xFFFF; ///< Default foreground colour, WHITE
    static const uint16_t WIDGET_COLOUR_DEFAULT_BG = 0x0000; ///< Default background colour, BLACK
    static const uint16_t WIDGET_COLOUR_DEFAULT_PRESSED = 0xD69A; ///< Default pressed colour, LIGHT_GREY
    static const uint16_t WIDGET_COLOUR_DEFAULT_DISABLED = 0x7BEF; ///< Default disabled foreground colout, DARK GREY

    /**
     * @brief Construct a derived widget with X, Y, width and height
     * 
     * @param x X position on the screen
     * @param y Y position on the screen
     * @param w Width of the widget
     * @param h Heightof the widget
     */
    BaseWidget(uint16_t  x, uint16_t  y, uint16_t  w, uint16_t  h) :
        Region(x, y, w, h)
        {}

    /**
     * @brief Construct a derived widget with a `Coords_s` struct, width and height
     * 
     * @param coords containing X and Y position
     * @param w Width of the widget
     * @param h Heightof the widget
     */
    BaseWidget(Coords_s coords, uint16_t  w, uint16_t  h) :
        Region(coords.x, coords.y, w, h)
        {}

    /**
     * @brief Construct a derived widget with width and height ( X and  Y default to 0 )
     * 
     * @param w Width of the widget
     * @param h Heightof the widget
     */
    BaseWidget(uint16_t  w, uint16_t  h) :
        Region(0, 0, w, h)
        {}

    /**
     * @brief Construct a derived widget from an existing `Region`s dimensions
     * 
     * @param r The source `Region`
     */
    BaseWidget(Region r) :
        Region(r)
        {}

    /**
     * @brief Use begin() to set up state and configuration that cannot be done in the constructor.
     * By default, this does nothing.
     */
    virtual void begin() {};

    /**
     * @brief Called when the screen starts to prepare for drawing and draw any non-changing elements
     */
    virtual void start() = 0;

    /**
     * @brief Draw the widget using current set values
     * 
     */
    virtual void draw() = 0;

    /**
     * @brief Clear the widget's Region
     * 
     */
    virtual void clear() = 0;

    /**
     * @brief Normally be called when the widget is not used in current screen
     * 
     */
    virtual void end() = 0;

    /**
     * @brief Derive classes need to decide what to do when state is changed
     * 
     * @param newState 
     */
    virtual void onStateChanged() = 0;

    /**
     * @brief Indicate that the widget requires re-drawing (usually because of a state change)
     * 
     * @param redraw Pass `true` (default) to set `isRedrawRequired()`
     */
    virtual void redrawRequired(bool redraw=true) { requiresRedraw = redraw;}

    /**
     * @brief Returns true if the `redrawRequired()` has been set to `true`
     * 
     * @return true Redraw is required
     * @return false Redraw is not required
     */
    bool isRedrawRequired() { return requiresRedraw; }

    /**
     * @brief Set the state of the widget. If the derived widget has custom states, create
     * an overloaded `setState(CustomStateEnum)`
     * 
     * @param newState 
     */
    void setState(WidgetDisplayState newState) {
        if ( newState == widgetState ) return;
        previousState = widgetState;
        widgetState = newState;
        onStateChanged();
    }

    /**
     * @brief Get the current state
     * 
     * @return WidgetDisplayState 
     */
    WidgetDisplayState getState() {
        return widgetState;
    }

    /**
     * @brief Convenience method
     * 
     * @param state 
     * @return true 
     * @return false 
     */
    bool isState(WidgetDisplayState state) {
        return widgetState == state;
    }
    /**
     * @brief Get the previous state
     * 
     * @return WidgetDisplayState 
     */
    WidgetDisplayState getPreviousState() {
        return previousState;
    }

    /**
     * @brief Convenience method
     * 
     * @return true 
     * @return false 
     */
    bool isHidden() { 
        return getState() == WidgetDisplayState::HIDDEN; 
    }


    /**
     * @brief Widget can use any colours, the foreground and background are provided for convenience.
     * 
     * @param colour 
     */
    virtual void setFgColour(uint16_t colour) {
        if ( colour == fgColour ) return;
        fgColour = colour;
        redrawRequired();
    }

    /**
     * @brief Return the foreground colour
     * 
     * @return uint16_t 
     */
    uint16_t getFgColour() { return fgColour; }

    /**
     * @brief Widget can use any colours, the foreground and background are provided for convenience.
     * 
     * @param colour 
     */
    virtual void setBgColour(uint16_t colour) {
        if ( colour == bgColour ) return;
        bgColour = colour;
        redrawRequired();
    }

    /**
     * @brief Return the background colour
     * 
     * @return uint16_t 
     */
    uint16_t getBgColour() { return bgColour; }

    ///@{
    /**
     * @name Widget ID and Value
     * @details These methods *are not* used internally and are for developer use to identify a particular widget, or hold an external state
     * Expected to be used with enums.
     */
    /**
     * @brief Set the widget ID (for use by user, not used internally). Not unique, default is 0.
     */
    void setWidgetId(uint8_t id) { widgetId = id; }

    /**
     * @brief Get the widget ID (for use by user, not used internally). Not unique, default is 0.
     */
    uint8_t getWidgetId() {return widgetId; }

    /**
     * @brief Set the widget value (for use by user, not used internally). Not unique, default is 0.
     */
    void setWidgetValue(uint8_t val) { widgetValue = val; }

    /**
     * @brief Get the widget value (for use by user, not used internally). Not unique, default is 0.
     */
    uint8_t getWidgetValue() { return widgetValue; }
    ///@}



    private:

    WidgetDisplayState widgetState = WidgetDisplayState::NONE; //
    WidgetDisplayState previousState = WidgetDisplayState::NONE; //
    bool requiresRedraw = true;
    uint16_t fgColour = WIDGET_COLOUR_DEFAULT_FG;
    uint16_t bgColour = WIDGET_COLOUR_DEFAULT_BG;
    uint8_t widgetId = 0; //
    uint8_t widgetValue = 0; //


};

} //namespace
#endif
/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_TOUCH_WIDGET_MIXIN_H
#define INPUT_EVENTS_TOUCH_WIDGET_MIXIN_H
#include "EventTouchScreen.h"

namespace input_events {

/**
 * @brief A mixin class for BaseWidget that can act on touch within its DisplayArea
 * 
 */
template <typename Derived>
class TouchWidgetMixin {

    public:

    /**
     * @brief Handle the touch event if appropriate. Return true if fully handled.
     * 
     * @details Note: This is intended for changing the internal state or drawing of the widget. If external
     * actions are required, use widget.contains(TouchPoint_s) (available for all widget types) or EventWidget
     * 
     * @param et 
     * @param touchPanel 
     * @return true If fully handled
     * @return false If not handled or not fully handled.
     */
    virtual bool onTouchEvent(InputEventType et, EventTouchScreen& touchPanel) = 0;


    protected:
    /**
     * @brief Protected default constructor to help prevent creation outside of a base derived widget
     * 
     */
    TouchWidgetMixin() {} 

    
};

}
#endif
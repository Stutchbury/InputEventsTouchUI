/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_VIRTUAL_PIN_WIDGET_MIXIN_H
#define INPUT_EVENTS_VIRTUAL_PIN_WIDGET_MIXIN_H

#include "PinAdapter/VirtualPinAdapter.h"

namespace input_events {

/**
 * @brief A mixin class for BaseWidget that holds a VirtualPinAdapter so it can be pressed and released 
 * 
 */
template <typename Derived>
class VirtualPinWidgetMixin {

    public:

    /**
     * @brief Set the VirtualPinAdapter for this derived widget
     * 
     * @param pinAdapter 
     */
    void setVirtualPin(VirtualPinAdapter* pinAdapter) {
        virtualPin = pinAdapter;
    }

    /**
     * @brief Check virtual pin exists and press it
     * 
     * @return true Virtual pin exists and has been pressed
     * @return false Virtual pin does not exist - no action taken
     */
    bool pressVirtualPin() {
        if ( virtualPin == nullptr ) return false;
        virtualPin->press();
        return true;
    }

    /**
     * @brief Check virtual pin exists and release it
     * 
     * @return true Virtual pin exists and has been released
     * @return false Virtual pin does not exist - no action taken
     */
    bool releaseVirtualPin() {
        if ( virtualPin == nullptr ) return false;
        virtualPin->release();
        return true;
    }

    /**
     * @brief Default behaviour for a virtual pin if a touch event occurs. Maps InputEventType::PRESSED and InputEventType::RELEASED.
     * 
     * @param et 
     * @param touchPanel 
     * @return true 
     * @return false 
     */
    bool virtualPinOnTouchEvent(InputEventType et, EventTouchScreen& touchPanel) {
        Derived* self = static_cast<Derived*>(this);
        if ( !self->isButtonEnabled()  && self->contains(touchPanel.getTouchPoint()) ) return true; //Handled by not being handled
        if ( et == InputEventType::PRESSED && self->contains(touchPanel.getTouchPoint()) ) {
            return pressVirtualPin();
        }
        if ( et == InputEventType::RELEASED && self->contains(touchPanel.getStartTouchPoint()) ) {
            return releaseVirtualPin();
        }
        return false;        
    }



    protected:
    /**
     * @brief Protected default constructor to help prevent creation outside of a base derived widget
     * 
     */
    VirtualPinWidgetMixin() {} 


    VirtualPinAdapter* virtualPin = nullptr; ///< A reference to a VirtualPinAdapter for this mixin


};

}
#endif
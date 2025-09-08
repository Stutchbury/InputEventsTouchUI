/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_BUTTON_LABEL_WIDGET_MIXIN_H
#define INPUT_EVENTS_BUTTON_LABEL_WIDGET_MIXIN_H

#include "EventButton.h"
#include "BaseWidget.h"

namespace input_events {

/**
 * @brief A mixin class for BaseWidget that links to a physical `EventButton` and has access to the state of that button. The `BaseWidget::draw()`
 * can then represent the state of the button on the display.
 * 
 * @details To have the label respond to touch, you will also need to derive from `TouchWidget` and to do virtual presses of the 
 * physical button, derive from `VirtualPinWidget` too. The `PinAdapter for the underlying `EventButton` needs to be a `PinMixerAdapter` 
 * using a regular pin type (usually `GpioPinAdapter`) and the `VirtualPinAdapter`.
 * 
 * The label effectively creates the 'type' of button, so the widget's state and the button's inputValue should probably be aligned. 
 * The button's input ID can also be linked to the widget.
 * 
 */
template <typename Derived>
class ButtonLabelWidgetMixin {

    public:

    /**
     * @brief Set widget to be a row label (0-4)
     * 
     * @param button 
     */
    void setEventButton(EventButton* button) {
        eventButton = button;
    }

    /**
     * @brief Set the underlying button ID (for use by user, not used internally). Not unique, default is 0. 
     * 
     * @param newId 
     */
    void setButtonId(uint8_t newId) {
        if ( eventButton == nullptr ) return;
        eventButton->setInputId(newId);
    }

    /**
     * @brief Get the underlying button ID (for use by user, not used internally). Not unique, default is 0. 
     * 
     * @return uint8_t 
     */
    uint8_t getButtonId() {
        if ( eventButton == nullptr ) return 0;
        return eventButton->getInputId();
    }

    /**
     * @brief Set the underlying button value (for use by user, not used internally). Not unique, default is 0. 
     * 
     * @param newValue 
     */
    void setButtonState(uint8_t newValue) {
        if ( eventButton == nullptr ) return;
        eventButton->setInputValue(newValue);
    }

    /**
     * @brief Get the underlying button value (for use by user, not used internally). Not unique, default is 0. 
     * 
     * @return uint8_t 
     */
    uint8_t getButtonValue() {
        if ( eventButton == nullptr ) return 0;
        return eventButton->getInputValue();
    }

    /**
     * @brief Returns true if the underlying button is currenty pressed
     * 
     * @return true 
     * @return false 
     */
    bool isButtonPressed() {
        if ( this->eventButton == nullptr ) return false;
        return this->eventButton->isPressed();
    }

    /**
     * @brief Returns true if the underlying button is currently enabled
     * 
     * @return true 
     * @return false 
     */
    bool isButtonEnabled() {
        if ( eventButton == nullptr ) return false;
        return eventButton->isEnabled();
    }

    /**
     * @brief Returns true if the underlying button is currently idle
     * 
     * @return true 
     * @return false 
     */
    bool isButtonIdle() {
        if ( eventButton == nullptr ) return false;
        return eventButton->isIdle();
    }

    protected:
    /**
     * @brief Protected default constructor to help prevent creation outside of a base derived widget
     * 
     */
    ButtonLabelWidgetMixin() {} 

    EventButton* eventButton = nullptr; ///< A reference to the underlying EventButton
};

} //namespace
#endif
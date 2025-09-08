/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_ICON_WIDGET_MIXIN_H
#define INPUT_EVENTS_ICON_WIDGET_MIXIN_H
#include "BaseIcon.h"

namespace input_events {

/**
 * @brief A mixin class for BaseWidget that draws an Icon
 * 
 */
template <typename Derived>
class IconWidgetMixin {

    public:

    /**
     * @brief Set the Icon to draw
     * 
     * @param newIcon 
     */
    void setIcon(const Icon& newIcon) {
        if (isIcon(newIcon)) return;
        icon = &newIcon;
        Derived* self = static_cast<Derived*>(this);
        self->redrawRequired();
    }

    /**
     * @brief Compare two icons
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    bool isIcon(const Icon& other) const {
        return icon == &other;
    }

    /**
     * @brief Set the IconParams
     * 
     * @param newIconParams 
     */
    void setIconParams(input_events::IconParams newIconParams) {
        iconParams = newIconParams;
    }

    /**
     * @brief Get the IconParams
     * 
     * @return input_events::IconParams 
     */
    input_events::IconParams getIconParams() {
        return iconParams;
    }

    protected:

    const Icon* icon = nullptr; ///< a reference to the icon
    input_events::IconParams iconParams; ///< a copy of the parameters


    protected:
    /**
     * @brief Protected default constructor to help prevent creation outside of a base derived widget
     * 
     */
    IconWidgetMixin() {} 

    
};

}
#endif
/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_WIDGET_ROW_CONTAINER_H
#define INPUT_EVENTS_WIDGET_ROW_CONTAINER_H

#ifndef WIDGET_CONTAINER_DEFAULT_MAX
/**
 * @brief The default maximum number of `BaseWidget`s in a `WidgetRowContainer`
 */
#define INPUT_EVENTS_WIDGET_CONTAINER_DEFAULT_MAX 5
#endif

#include <Arduino.h>
#include "WidgetContainer.h"

namespace input_events {

/**
 * @brief A WidgetRowContainer has similar functionality to the WidgetContainer but provides the ability to alter the Region (size/position) of contained widgets
 * based on the row container Region and assigned position of the child widget.
 * 
 */
template<size_t maxWidgets = INPUT_EVENTS_WIDGET_CONTAINER_DEFAULT_MAX>
class WidgetRowContainer  : public WidgetContainer<maxWidgets> {

    public:

    using Base = WidgetContainer<maxWidgets>; ///< Make life easier

    /**
     * @brief Construct a WidgetRowContainer using a Region
     * 
     * @param region 
     */
    explicit WidgetRowContainer(Region region ) : 
        WidgetContainer<maxWidgets>(region),
        widenLast(true)
        {}

    /**
     * @brief Construct a WidgetRowContainerusing x, y, width and height
     * 
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     */
    WidgetRowContainer(uint16_t x, uint16_t y, uint16_t w, uint16_t h ) : 
        WidgetContainer<maxWidgets>(x, y, w, h),
        widenLast(true)
        {}

    /**
     * @brief Add a widget to the row container in position 
     * 
     * @details The position is only used to calculate the x, y, w, h for the contained widget
     * 
     * @param widget 
     * @param position 
     * @return size_t 
     */
    size_t add(BaseWidget* widget, uint8_t position ) {
        size_t newIndex = WidgetContainer<maxWidgets>::addWidget(widget);
        if ( newIndex == Base::InvalidIndex ) return Base::InvalidIndex;
        setWidgetRegion(widget, position);
        return newIndex;
    }

    /**
     * @brief Calculate and set the Region for widget at index for position (not normally called directly)
     * 
     * @param index 
     * @param position 
     */
    void setWidgetRegion(size_t index, uint8_t position ) {
        if ( index >= this->count ) return;
        if ( position >= maxWidgets ) return;
        if ( !this->widgets[position] ) return;
        setWidgetRegion(this->widgets[index], position);
    }

    /**
     * @brief Calculate and set the Region for the passed widget for position (not normally called directly)
     * 
     * @param widget 
     * @param position 
     */
    void setWidgetRegion(BaseWidget* widget, uint8_t position ) {
        if ( !widget ) return;
        if ( position >= maxWidgets ) return;
        widget->setX( this->xDiv(maxWidgets, position));
        widget->setY(this->y());
        if ( widenLast && position == maxWidgets-1 ) {
            widget->setW((this->r() - widget->x()) + 1 );
        } else {
            widget->setW( this->wDiv(maxWidgets));
        }
        widget->setH(this->h());
    }

    protected:

    private:
    bool widenLast = true;

};

} //namespace
#endif
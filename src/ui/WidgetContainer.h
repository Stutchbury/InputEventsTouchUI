/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_WIDGET_CONTAINER_H
#define INPUT_EVENTS_WIDGET_CONTAINER_H

#ifndef WIDGET_CONTAINER_DEFAULT_MAX
/**
 * @brief The default maximum number of `BaseWidget`s in a `WidgetContainer`
 */
#define WIDGET_CONTAINER_DEFAULT_MAX 5
#endif

#include <Arduino.h>
#include "BaseWidget.h"

namespace input_events {

/**
 * @brief A widget container that will draw() all added widgets if not WidgetDisplayState::HIDDEN and always call all widgets begin() and end()
 * 
 */
template<size_t maxWidgets = INPUT_EVENTS_WIDGET_CONTAINER_DEFAULT_MAX>
class WidgetContainer  : public BaseWidget {

    public:

    //using Index = size_t; 
    static constexpr size_t InvalidIndex = static_cast<size_t>(-1); ///< Define invalid value as max size

    /**
     * @brief Construct a WidgetContainer using a Region
     * 
     * @param region 
     */
    explicit WidgetContainer(Region region ) : 
        BaseWidget(region)
        {}

    /**
     * @brief Construct a WidgetContainer using x, y, width and height
     * 
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     */
    WidgetContainer(uint16_t x, uint16_t y, uint16_t w, uint16_t h ) : 
        BaseWidget(x, y, w, h)
        {}

    /**
     * @brief If not hidden, call begin() of all contained widgets
     * 
     */
    void begin() override { //Loop through widgets
        for (size_t i = 0; i < count; ++i) {
            if ( widgets[i] ) {
                widgets[i]->begin();
            }
        }
    }

    /**
     * @brief If not hidden, call start() of all contained widgets
     * 
     */
    void start() override { //Loop through widgets
        for (size_t i = 0; i < count; ++i) {
            if ( widgets[i] ) {
                widgets[i]->start();
            }
        }
        redrawRequired();
    }

    /**
     * @brief If not hidden, call redrawRequired() of all contained widgets
     * 
     * @param redraw 
     */
    void redrawRequired(bool redraw=true) override { 
        BaseWidget::redrawRequired(redraw);
        for (size_t i = 0; i < count; ++i) {
            if ( widgets[i] ) {
                widgets[i]->redrawRequired(redraw);
            }
        }
    }

    /**
     * @brief If not hidden, call draw() of all contained widgets
     * 
     */
    void draw() override { //Loop through widgets if not HIDDEN
        if ( this->isHidden() ) return;
        for (size_t i = 0; i < count; ++i) {
            if ( widgets[i] ) {
                widgets[i]->draw();
            }
        }
    }

    /**
     * @brief If not hidden, call clear() of all contained widgets
     * 
     */
    void clear() override { //Loop through widgets if not HIDDEN
        if ( this->isHidden() ) return;
        for (size_t i = 0; i < count; ++i) {
            if ( widgets[i] ) {
                widgets[i]->clear();
            }
        }
    }

    /**
     * @brief Call end() of all contained widgets
     * 
     */
    void end() override { //Loop through widgets
        for (size_t i = 0; i < count; ++i) {
            if ( widgets[i] ) {
                widgets[i]->end();
            }
        }
    }

    /**
     * @brief If state is not hidden, set all contained widgets to be redrawn
     * 
     */
    void onStateChanged() override {
        if ( getState() == WidgetDisplayState::HIDDEN ) return;
        redrawRequired(); //Will set all widgets redrawRequired()
    }


    /**
     * @brief Add a widget to the container
     * 
     * @param widget 
     * @return size_t 
     */
    size_t addWidget(BaseWidget* widget) {
        if (count >= maxWidgets)
            return InvalidIndex;
        widgets[count] = widget;
        return count++;  // Return the index before incrementing
    }

    /**
     * @brief Remove the widget at index.
     * 
     * @param index 
     */
    void removeWidget(size_t index) {
        if (index >= count) return;
        // Shift widgets down to fill the gap
        for (size_t i = index; i < count - 1; ++i) {
            widgets[i] = widgets[i + 1];
        }
        --count;
        widgets[count] = nullptr;
    }

    /**
     * @brief Remove the widget
     * 
     * @param widget 
     */
    void removeWidget(BaseWidget* widget) {
        for (size_t i = 0; i < count; ++i) {
            if (widgets[i] == widget) {
                removeWidget(i);
                return;
            }
        }
    }

    /**
     * @brief Remove all contained widgets
     * 
     */
    void removeAllWidgets() {
        for (size_t i = 0; i < count; ++i) {
            widgets[i] = nullptr;
        }
        count = 0;
    }

    /**
     * @brief Replace widget at index with newWidget
     * 
     * @param index 
     * @param newWidget 
     * @return true 
     * @return false 
     */
    bool replaceWidget(size_t index, BaseWidget* newWidget) {
        if (index >= count) {
            return false;  // Invalid index
        }
        widgets[index] = newWidget;
        return true;
    }

    /**
     * @brief Replace a oldWidget with newWidget 
     * 
     * @param oldWidget 
     * @param newWidget 
     * @return true 
     * @return false 
     */
    bool replaceWidget(BaseWidget* oldWidget, BaseWidget* newWidget) {
        for (size_t i = 0; i < count; ++i) {
            if (widgets[i] == oldWidget) {
                widgets[i] = newWidget;
                return true;
            }
        }
        return false;
    }    

    /**
     * @brief Return the number of contained widgets
     * 
     * @return size_t 
     */
    size_t size() const { return count; }


    protected:

    BaseWidget* widgets[maxWidgets]; ///< the contained widgets
    size_t count = 0; ///< Number of added widgets

};

} //namespace
#endif
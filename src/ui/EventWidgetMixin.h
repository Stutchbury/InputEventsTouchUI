/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_EVENT_WIDGET_MIXIN_H
#define INPUT_EVENTS_EVENT_WIDGET_MIXIN_H

#include "Region.h"
#include "EventTouchScreen.h"

namespace input_events {

//Forward declare...
class EventTouchScreen;

/**
 * @brief A mixin class for BaseWidget that can fire events
 * 
 * @tparam Derived 
 */
template <typename Derived>
class EventWidgetMixin {

    protected:

    #if defined(FUNCTIONAL_SUPPORTED)
    /**
     * @brief If <code>std::function</code> is supported, use to create the callback type.
     */
    typedef std::function<void(InputEventType et, Derived &ts)> CallbackFunction;
    #else
    /**
     * @brief Create the callback type as pointer if <code>std::function</code> is not supported.
     */
    typedef void (*CallbackFunction)(InputEventType et, Derived &);
    #endif

    /**
     * @brief The callback function member.
     */
    CallbackFunction callbackFunction = nullptr;


    public:

    /**
     * @brief Set the Callback function.
     * 
     * @param f A function of type <code>EventTouchScreen::CallbackFunction</code> type.
     */
    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    #if defined(FUNCTIONAL_SUPPORTED)
    /**
     * @brief Set the Callback function to a class method.
     * 
     * @details Note: This method is only available if <code>std:function</code> is supported.
     * 
     * 
     * @param instance The instance of a class implementing a CallbackFunction method.
     * @param method The class method of type <code>DerivedWidget::CallbackFunction</code> type.
     */
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, Derived&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, Derived& ie) {
            (instance->*method)(et, ie); // Call the member function on the instance
        };
        callbackIsSet = true;
    }
    #else
    /// \cond DO_NOT_DOCUMENT
    template <typename T>
    void setCallback(T*, void (T::*)(InputEventType, Derived&)) {
        static_assert(sizeof(T) == 0, "std::function is not supported on this platform. Use setCallback(FunctionPointer) instead.");
    }
    /// \endcond
    #endif

    /**
     * @brief Unset a previously set callback function or method.
     * 
     * @details Must be called if the previously set method is destroyed.
     * 
     */
    void unsetCallback() {
        callbackFunction = nullptr;
        callbackIsSet = false;
    }

    /**
     * @brief Invoke the callback function (public, but not normally called directly)
     * 
     * @param et 
     * @return true 
     * @return false 
     */
    bool invoke(InputEventType et) {
        if ( !callbackFunction ) return false;
        Derived* self = static_cast<Derived*>(this);
        callbackFunction(et, *self);
        return true;
    }

    protected:
    bool callbackIsSet = false; ///< Set to true when callback is valid

};



} //namespace
#endif
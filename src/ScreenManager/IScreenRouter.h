#ifndef INPUT_EVENTS_ISCREEN_ROUTER_H
#define INPUT_EVENTS_ISCREEN_ROUTER_H

#if defined(__has_include)
#if __has_include(<vector>) && __has_include(<string>) //Check if std lib is supported

#include <Arduino.h>
#include <string>
#include "ScreenTransition.h"

namespace input_events {

/** \ingroup ScreenManager
 *  @{
 */

/**
 * @brief The interface for a class that resolves screen transitions (ie moving Next, Back or to a new screen). Called by EventScreenManager if requestScreen is set.
 * 
 */
class IScreenRouter {
public:
    virtual ~IScreenRouter() = default;

    /**
     * @brief Resolve a scren transition intent's validity. Can return an empty string (no objection) or a redirect
     * 
     * @param current 
     * @param intent 
     * @return std::string 
     */
    virtual std::string resolveScreen(const std::string& current,
                                const TransitionIntent& intent) = 0;
};

/** @}*/

} // namespace

#endif
#endif

#endif

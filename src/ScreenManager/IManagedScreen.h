#ifndef INPUT_EVENTS_IMANAGED_SCREEN_H
#define INPUT_EVENTS_IMANAGED_SCREEN_H

#if defined(__has_include)
#if __has_include(<vector>) && __has_include(<string>) //Check if std lib is supported

#include <Arduino.h>

#include <string>

namespace input_events {

/** \ingroup ScreenManager
 *  @{
 */

 /**
 * @brief Interface for a screen that can be managed by EventScreenManager. A 'Screen' is a combination of the 'View' part of MVC (Model, View, Controller) pattern with a portion of 'Controller mixed in.
 * 
 * @details Widgets and Icons are the true 'View' components but they are called from a Screen.
 * 
 */
class IManagedScreen {

    public:

    /**
     * @brief The string name of the screen
     * 
     * @return std::string 
     */
    virtual std::string name() const = 0;
    
    /**
     * @brief Set the Alias for this screen. Called by EventScreenManager when screen is added
     * 
     * @param id 
     */
    virtual void setId(const std::string& id) = 0;

    /**
     * @brief return the id (often the Name) used to register the screen
     * 
     * @return const std::string& 
     */
    virtual const std::string& id() const = 0;

    /**
     * @brief Called immediately after the screen is added via `ScreenManager::registerScreen()`
     */
    virtual void begin() = 0;

    /**
     * @brief Called by `EventScreenManager` when this screen is set active
     */
    virtual void start() = 0;

    /**
     * @brief Called at defined FPS only whan screen is active
     */
    virtual void draw() = 0;

    /**
     * @brief Called by `EventScreenManager` before the next screen is set active
     * 
     */
    virtual void end() = 0;

};

/** @}*/

}

#endif
#endif

#endif
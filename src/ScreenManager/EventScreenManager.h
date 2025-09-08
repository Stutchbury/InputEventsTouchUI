#ifndef INPUT_EVENTS_EVENT_SCREEN_MANAGER_H
#define INPUT_EVENTS_EVENT_SCREEN_MANAGER_H

#if defined(__has_include)
#if __has_include(<vector>) && __has_include(<string>) //Check if std lib is supported

#include <Arduino.h>
#include <unordered_map>
#include <string>
#include "IScreenRouter.h"
#include "IManagedScreen.h"


namespace input_events {

/** \defgroup ScreenManager Screen Manager
 *  @{
 */

/**
 * @brief A ScreenManager for frameworks the support the std lib (not currently AVR)
 * 
 */
class EventScreenManager {

public:


    /**
     * @brief Construct a new EventScreenManager
     * 
     * @param appPtr 
     */
    explicit EventScreenManager() { }


    /**
     * @brief Called once during setup()
     * 
     */
    void begin() { 
        pendingIntent = { TransitionIntentType::Init };
    }

    /**
     * @brief Called from loo(). Checks if a screen transition has been requested and calls current screen's draw() at set FPS.
     * 
     */
    void update() {
        if ( pendingIntent.type != TransitionIntentType::None ) {
            resolveTransition(pendingIntent);
            pendingIntent = {};
        }
        now = millis();
        if ((uint32_t)(now - lastDisplayRefresh) < displayRefreshMs) return;
        lastDisplayRefresh = now;
        if (current) current->draw();
    }

    /**
     * @brief Register an IManagedScreen. Uses screen's name() if id is not provided.
     * 
     * @param screen 
     * @param id 
     * @return true 
     * @return false 
     */
    bool registerScreen(IManagedScreen* screen, std::string id = {} ) {
        id = ( id.empty() ? screen->name() : id );
        if ( id.empty() ) return false; //In case of empty name() and id
        if ( screens.empty() ) initialScreen = id;
        auto result = screens.emplace(id, screen);
        if (!result.second) return false; //Already exists
        screen->setId(id);
        screen->begin();
        return true;
    }

    /**
     * @brief Overwrite an already registered screen
     * 
     * @param screen 
     * @param id 
     * @return true 
     * @return false 
     */
    bool overwriteScreen(IManagedScreen* screen, std::string id = {} ) {
        id = ( id.empty() ? screen->name() : id );
        if ( id.empty() ) return false; //In case of empty name() and id
        screens[id] = screen; //Overwrite
        screen->setId(id);
        screen->begin();
        return true;
    }

    /**
     * @brief Add a new router for screen transitions. Routers are called in the order they are added. First to answer wins.
     * 
     * @param router 
     */
    void addRouter(IScreenRouter* router) { routers.push_back(router); }


    /**
     * @brief Request a transition to a named screen. 
     * 
     * @details Will be passed to the screen routers (if any) for validation and/or redirect.
     * 
     * @param nextScreen 
     */
    void requestScreen(const std::string& nextScreen) {
        requestScreen( { TransitionIntentType::Next, nextScreen } );
    }

    /**
     * @brief Request a screen transition.
     * 
     * @details The request will be passed to the screen routers (if any) for validation and/or redirect.
     * 
     * @param intent Can be Init, Back, Next, Auto or a specific screen name. Screen router resolves screen name.
     */
    void requestScreen(const TransitionIntent& intent) { 
        pendingIntent = intent; 
    }
    
    /**
     * @brief Get a Screen object (not necessarily the current one)
     * 
     * @param id The name or id used to register the screen
     * @return IManagedScreen*  or nullptr if screen doesn't exist
     */
    IManagedScreen* getScreen(const std::string& id) {
        auto it = screens.find(id);
        if (it == screens.end()) return nullptr;
        if ( !it->second ) return nullptr;
        return it->second;
    }

    /**
     * @brief Check is a screen has been added and exists.
     * 
     * @param id The name or id used to register the screen
     * @return true Screen is available to get.
     * @return false Screen does not exist/
     */
    bool haveScreen(const std::string& id) {
        auto it = screens.find(id);
        if (it == screens.end()) return false;
        if ( !it->second ) return false;
        return true;
    }

    /**
     * @brief Returns true if passed screen is the current screen
     * 
     * @param screen 
     * @return true 
     * @return false 
     */
    bool isCurrent(const IManagedScreen* screen ) {
        if ( !current ) return false;
        if ( screen == nullptr ) return false;
        return screen == current;
    }

    /**
     * @brief Return true if passed id is that of the current screen
     * 
     * @param id 
     * @return true 
     * @return false 
     */
    bool isCurrent(const std::string& id) {
        if ( !current ) return false;
        auto it = screens.find(id);
        if (it == screens.end()) return false;
        return ( it->second == current );
    }

    /**
     * @brief Returns true if passed screen is the previous screen
     * 
     * @param screen 
     * @return true 
     * @return false 
     */
    bool isPrevious(const IManagedScreen* screen ) {
        if ( !previous ) return false;
        if ( screen == nullptr ) return false;
        return screen == previous;
    }

    /**
     * @brief Return true if passed id is that of the previous screen
     * 
     * @param id 
     * @return true 
     * @return false 
     */
    bool isPrevious(const std::string& id) {
        if ( !previous ) return false;
        auto it = screens.find(id);
        if (it == screens.end()) return false;
        return ( it->second == previous );
    }

    /**
     * @brief Get the current screen
     * 
     * @return IManagedScreen* or nullptr
     */
    IManagedScreen* getCurrent() {
        if (!current) return nullptr; 
        return current;
    }


    /**
     * @brief Get the previous screen
     * 
     * @return IManagedScreen* or nullptr
     */
    IManagedScreen* getPrevious() {
        if (!previous) return nullptr; 
        return previous;
    }

    /**
     * @brief Set the FPS (Frames Per Second) for draw() to be called
     * 
     * @param fps 
     */
    void setFps(uint8_t fps) {
        this->displayRefreshMs = (uint16_t)(1000/fps);
    }

    /**
     * @brief Get the currently set Fps
     * 
     * @return uint8_t 
     */
    uint8_t getFps() {
        return (uint8_t)(displayRefreshMs/1000);
    }

private:

    void resolveTransition(const TransitionIntent& intent) {
        if ( screens.empty() ) return;

        std::string resolved;

        if ( routers.empty() ) {
            if ( intent.type == TransitionIntentType::Init ) { 
                resolved = screens.empty() ? "" : initialScreen; //Init with first registered screen
            }
        } else { // Ask routers in priority order for the next screen
            for (auto router : routers) {
                resolved = router->resolveScreen(current ?  current->id() : std::string{}, intent);
                if (!resolved.empty() ) break; // take the first valid result
                if ( intent.type == TransitionIntentType::Init ) { //Should be resolved by first router, but if not, use first registered screen
                    resolved = screens.empty() ? "" : initialScreen;
                    break; //Only first router can resolve Init
                }
            }
        }

        if (resolved.empty() && !intent.requested.empty() ) { //No router complained, so just use the requested screen
            resolved = intent.requested;
        }
        if ( resolved.empty() ) return; //final sanity check
        // Find the screen in the map
        auto it = screens.find(resolved);
        if (it == screens.end()) return; // Screen not registered
        IManagedScreen* nextScreen = it->second;
        if (current) {
            if (nextScreen == current) return; // Already on this screen
            current->end();
            previous = current;
        }
        current = nextScreen;
        current->start();
    }


    std::unordered_map<std::string, IManagedScreen*> screens;
    std::vector<IScreenRouter*> routers;
    IManagedScreen* current = nullptr;
    IManagedScreen* previous = nullptr;
    TransitionIntent pendingIntent;
    uint16_t displayRefreshMs = 100;
    uint32_t now = millis();
    uint32_t lastDisplayRefresh = 0;
    std::string initialScreen = ""; //Because C/C++ map->begin() doesn't return first insertion and no 'orderedmap'... \_0_/

};

/** @}*/


}


#endif
#endif //

#endif

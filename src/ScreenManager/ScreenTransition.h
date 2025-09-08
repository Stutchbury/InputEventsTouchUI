#ifndef INPUT_EVENTS_SCREEN_TRANSITION_H
#define INPUT_EVENTS_SCREEN_TRANSITION_H

#if defined(__has_include)
#if __has_include(<vector>) && __has_include(<string>) //Check if std lib is supported

#include <Arduino.h>
#include <string>

namespace input_events {

/** \ingroup ScreenManager
 *  @{
 */

/**
 * @brief Types of screen transition
 * 
 */
enum class TransitionIntentType : uint8_t { 
    None, ///< No trnsition, stay on current screen
    Auto, ///< The IScreenRouter decides which screen to transition to.
    Next, ///< The IScreenRouter can accept a provided screen name or redirect
    Back, ///< The IScreenRouter determines what the previous screen should be
    Init  ///< Only the first IScreenRouter can resolve this.
};

/**
 * @brief The type of transition and an optional requested screen name
 * 
 */
struct TransitionIntent {
    TransitionIntentType type{TransitionIntentType::None}; ///< The type of requested transition
    std::string requested; ///< optional requested screen id
};

/** @}*/

} // namespace

#endif
#endif

#endif

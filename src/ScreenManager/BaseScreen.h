#ifndef INPUT_EVENTS_BASE_SCREEN_H
#define INPUT_EVENTS_BASE_SCREEN_H

#if defined(__has_include)
#if __has_include(<vector>) && __has_include(<string>) //Check if std lib is supported

#include "IManagedScreen.h"

namespace input_events {

/** \ingroup ScreenManager
 *  @{
 */

/**
 * @brief The Base screen that can be managed by EventScreenManager
 * 
 */
class BaseScreen : public IManagedScreen {

public:
    virtual void setId(const std::string& id) override {
        alias_ = id;
    }

    virtual const std::string& id() const override {
        return alias_;
    }

private:
    std::string alias_ = "";

};

/** @}*/

} // namespace

#endif

#endif
#endif




# Icons

To use icons, in your icon definition file (I tend to put all icons in one .h/.cpp):

```
#include <ui/Region.h>
#include <ui/BaseIcon.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft; 

class IconPlay : public input_events::BaseIcon {
    public:
    void drawIcon(input_events::Region region, input_events::IconParams params ) const override {
        //Draw your icon here (IconParams will have been applied)
    }
};

. . . 
rinse & repeat for each icon
. . .

```

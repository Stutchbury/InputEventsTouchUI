# InputEventsTouchUI Examples

Before running the examples, I strongly recommend testing the touch adapter's underlying library (eg Adafruit's TouchScreen `touchscreendemo.ino`).

The examples show the 'building blocks:

1. `AdafruitTouchScreenAdapterSerial.ino` creates a TouchScreenAdapter and prints output to the Serial Monitor.
2. `AdafruitEvenTouchScreen.ino` creates an EventTouchScreen and prints output to the Serial Monitor.
3. `AdafruitResistiveTouchScreenGFXILI9341.ino` is the first example to actually print anything to a screen!




## Notes on using Adafruit's GFX with Teensy on PlatformIO

The specific combination of Adafruit's GFX+Teensy *on PlatformIO* will likely cause a linker error: `multiple definition of operator delete(void*, unsigned int)`. Adafruit GFX+Teensy on the Arduino IDE will compile just fine...

[This issue](https://forum.pjrc.com/index.php?threads/multiple-definition-of-error-when-using-std-function-with-argument-teensy41-platformio.74505/) appears to be the root cause and by his replies, is unlikely to be fixed by Paul Stoffregen because the issue cannot be replicated on the Arduino IDE. Disappointing, but it's his project.

This is the workaround I have used to exclude the Teensy `new.cpp`: https://community.platformio.org/t/excluding-arduino-framework-new-cpp-from-the-build/20071

The ScreenManager module currently uses the `std` library which is likely the trigger (not the cause) of this issue - I may change that in the future.
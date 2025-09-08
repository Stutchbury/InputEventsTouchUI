# ScreenManager

Please note: ScreenManager requires the `std` library to be available for your board/framework - if it is not, the files will be excluded.

The detection of the `std` library is done with:

```#if defined(__has_include)
  #if __has_include(<vector>) && __has_include(<string>) //Check if std lib is supported
. . . 
#endif
#endif
```

# Feather_TFT_Scrolling
Example of text scrolling with the Adafruit TFT FeatherWing.

The Adafruit_GFX and Adafruit_ILI9341 are great for quickly bringing up the TFT display, but text scrolling is not supported by default. This example allows writing lines of text in various sizes while scrolling the display using the scrollTo() function (ILI9341_VSCRSADD command).

Note: this works fine in portrait mode, but will not work in landscape. In order to implement this, a lot of work would need to be done in the library itself. This example uses the library as-is. In addition, this method doesn't support "windowing" (stationary text above or below the scrolling text). This again, would require significant work in the library (which I would love to see done).


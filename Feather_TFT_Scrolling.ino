/*
 Name:      Feather_TFT_Scrolling.ino
 Created:   7/31/2017 10:14:42 AM
 Author:    joe
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <stdarg.h>

#ifdef ESP8266
#define STMPE_CS 16
#define TFT_CS   0
#define TFT_DC   15
#define SD_CS    2
#endif
#ifdef __AVR_ATmega32U4__
#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5
#endif
#ifdef ARDUINO_SAMD_FEATHER_M0
#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5
#endif
#ifdef TEENSYDUINO
#define TFT_DC   10
#define TFT_CS   4
#define STMPE_CS 3
#define SD_CS    8
#endif
#ifdef ARDUINO_STM32_FEATHER
#define TFT_DC   PB4
#define TFT_CS   PA15
#define STMPE_CS PC7
#define SD_CS    PC5
#endif
#ifdef ARDUINO_NRF52_FEATHER /* BSP 0.6.5 and higher! */
#define TFT_DC   11
#define TFT_CS   31
#define STMPE_CS 30
#define SD_CS    27
#endif

#define TEXT_HEIGHT_UNSCALED    8
#define PRINTFLN_MAXLEN         128

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

uint32_t LoopTimer = 0;
uint16_t LoopTime = 1000;

void tft_printfln(uint8_t size, uint16_t fg_color, uint16_t bg_color, char *fmt, ...)
{
    static uint16_t line = 0;
    static bool scroll = false;

    // Resulting string limited to PRINTFLN_MAXLEN chars
    char buf[PRINTFLN_MAXLEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, PRINTFLN_MAXLEN, fmt, args);
    va_end(args);

    tft.setTextColor(fg_color, bg_color);
    tft.setTextSize(size);

    // Handle repositioning in the framebuffer
    if (line >= tft.height() / TEXT_HEIGHT_UNSCALED)
    {
        scroll = true;
        line = 0;
        tft.setCursor(0, 0);
    }

    if (scroll)
    {
        tft.fillRect(0, (TEXT_HEIGHT_UNSCALED * line), tft.width(), (TEXT_HEIGHT_UNSCALED * size), bg_color);
        tft.scrollTo((TEXT_HEIGHT_UNSCALED * line) + (TEXT_HEIGHT_UNSCALED * size));
    }

    tft.println(buf);

    line += size;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("ILI9341 Scrolling Example");

    tft.begin();
    tft.fillScreen(ILI9341_BLACK);
    tft.setRotation(0);
    tft.setTextWrap(false);

    // Display some diagnostics highlighted in larger font to start
    uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    tft_printfln(2, ILI9341_BLACK, ILI9341_GREEN, "Power Mode:   0x%02X  ", x);
    x = tft.readcommand8(ILI9341_RDMADCTL);
    tft_printfln(2, ILI9341_BLACK, ILI9341_GREEN, "MADCTL Mode:  0x%02X  ", x);
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    tft_printfln(2, ILI9341_BLACK, ILI9341_GREEN, "Pixel Format: 0x%02X  ", x);
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    tft_printfln(2, ILI9341_BLACK, ILI9341_GREEN, "Image Format: 0x%02X  ", x);
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    tft_printfln(2, ILI9341_BLACK, ILI9341_GREEN, "Self Diag:    0x%02X  ", x);
}

void loop()
{
    uint32_t mil = millis();

    if (mil > LoopTimer)
    {
        uint8_t  size  = 1;
        uint16_t color = ILI9341_GREEN;

        // Highlight output from 34th second
        if (mil > 34000 && mil < 35000)
        {
            size = 2;
            color = ILI9341_RED;
        }

        LoopTimer += LoopTime;

        tft_printfln(size, color, ILI9341_BLACK, "millis == %lu", mil);
    }
}


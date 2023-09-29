#ifndef TFTDisplay_h_
#define TFTDisplay_h_

#include <TFT_eSPI.h>
#include <SPI.h>

#define WITH_DISPLAY
// #define NO_DISPLAY

class TFTDisplay : public TFT_eSPI
{
public:
    TFTDisplay();
    void clear();
    void printAt(String text, int x, int y, int size, uint16_t color, uint16_t backgroundColor);
    void printAt(String text, int x, int y, int size);
    void printAt(String text, int x, int y);

private:
    TFT_eSPI *_tft;
};

#endif

#pragma once

#include "TFTDisplay.h"

#ifdef NO_DISPLAY
#include "TFTDisplay.h"
#include <TFT_eSPI.h>

TFTDisplay::TFTDisplay()
{
}

void clear() {}
void printAt(String text, int x, int y, int size, uint16_t color, uint16_t backgroundColor) {}
void printAt(String text, int x, int y, int size) {}
void printAt(String text, int x, int y) {}
#endif
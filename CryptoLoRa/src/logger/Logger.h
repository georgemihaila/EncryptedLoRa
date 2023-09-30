#pragma once

#include <Arduino.h>
#include "../TFT/TFTDisplay.h"

class Logger
{
private:
  TFTDisplay *_display;
  int _x;
  int _y;
  int _lineHeight;
  void _incrementY();
  void _print(String message);
  void _clearLine(int y);

public:
  Logger(TFTDisplay *display);
  void info(String message);
};
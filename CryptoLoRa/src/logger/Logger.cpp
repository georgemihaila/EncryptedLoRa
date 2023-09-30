#include "Logger.h"

Logger::Logger(TFTDisplay *display)
{
  _display = display;
  _lineHeight = 20;
  _x = 0;
  _y = -_lineHeight;
}

void Logger::info(String message)
{
  Serial.println(message);
  _print(message);
}

void Logger::_print(String message)
{
  _incrementY();
  _clearLine(_y);
  _display->printAt(message, _x, _y);
}

void Logger::_incrementY()
{
  _y += _lineHeight;
  if (_y >= TFT_HEIGHT / 2 - _lineHeight)
  {
    _y = 0;
  }
}

void Logger::_clearLine(int y)
{
  _display->fillRect(0, y, TFT_WIDTH, _lineHeight, TFT_BLACK);
}
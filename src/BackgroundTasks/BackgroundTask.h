#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <functional>

class BackgroundTask
{
public:
  BackgroundTask(const char *taskName, std::function<void()> callbackFunction);
  void start();
  void stop();

private:
  TaskHandle_t taskHandle;
  std::function<void()> _callbackFunction;
  const char *_taskName;
};

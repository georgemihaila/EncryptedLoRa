#include "BackgroundTask.h"
#include <esp32-hal.h>

BackgroundTask::BackgroundTask(const char *taskName, std::function<void()> callback)
{
  taskHandle = NULL;
  _callbackFunction = callback;
  _taskName = taskName;
}

void BackgroundTask::start()
{
  xTaskCreate([](void *param)
              {
        BackgroundTask *task = (BackgroundTask *)param;
        while (1) {
            if (task->_callbackFunction != NULL) {
                task->_callbackFunction();
            }
            vTaskDelay(pdMS_TO_TICKS(500));
            yield();
        } },
              _taskName, 4096, this, 1, &taskHandle);
}

void BackgroundTask::stop()
{
  if (taskHandle != NULL)
  {
    vTaskDelete(taskHandle);
    taskHandle = NULL;
  }
}
#pragma once

#include <Arduino.h>
#include "Payload.h"
#include <iostream>
#include <miniz.h>

namespace Data
{
  /*
   * @brief (De)compression utilities. Uses zlib's gzip.
   */
    class Compressor
    {
    public:
        static String deflate(String message);
        static String inflate(String message);
    };
};
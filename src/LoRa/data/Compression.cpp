#pragma once

#include "Compression.h"

namespace Data
{
  namespace Compression
  {
    String Data::Compression::deflate(String message)
    {
      // Define buffers and their sizes
      uint8_t inBuffer[message.length()];
      uint8_t outBuffer[message.length() * 2]; // Output buffer should be larger
      size_t inSize = message.length();
      size_t outSize = sizeof(outBuffer);

      // Copy the message into the input buffer
      memcpy(inBuffer, message.c_str(), inSize);

      // Compress the data
      mz_ulong compressedSize = outSize;
      int result = mz_compress(outBuffer, &compressedSize, inBuffer, inSize);

      if (result != MZ_OK)
      {
        String errorMessage = "Compression failed with error code: " + String(result);
        return errorMessage;
      }

      // Convert compressed buffer to String and return
      return String((char *)outBuffer).substring(0, compressedSize);
    }

    // Decompress a given message
    String Data::Compression::inflate(String message)
    {
      // Define buffers and their sizes
      uint8_t inBuffer[message.length()];
      uint8_t outBuffer[message.length() * 2];
      size_t inSize = message.length();
      size_t outSize = sizeof(outBuffer);
      // Copy the message into the input buffer
      memcpy(inBuffer, message.c_str(), inSize);

      // Decompress the data
      mz_ulong decompressedSize = outSize;
      if (mz_uncompress(outBuffer, &decompressedSize, inBuffer, inSize) != MZ_OK)
      {
        return "Decompression failed";
      }

      // Convert decompressed buffer to String and return
      return String((char *)outBuffer).substring(0, decompressedSize);
    }
  };
};
#pragma once

#include "Crypto.h"
#include <Arduino.h>

namespace Crypto
{
  class AES128
  {
  private:
    inline static Cipher *_cipher = new Cipher();

  public:
    static String encrypt(char *message, char *key);
    static String encrypt(String message, String key);
    static String decrypt(String message, String key);
    static String decrypt(char *message, char *key);
  };
};
#pragma once

#include "AES128.h"

namespace Crypto
{
  String AES128::encrypt(char *message, char *key)
  {
    _cipher->setKey(key);
    return _cipher->encryptString(message);
  }

  String AES128::encrypt(String message, String key)
  {
    return encrypt((char *)message.c_str(), (char *)key.c_str());
  }

  String AES128::decrypt(String message, String key)
  {
    return decrypt((char *)message.c_str(), (char *)key.c_str());
  }

  String AES128::decrypt(char *message, char *key)
  {
    _cipher->setKey(key);
    return _cipher->decryptString(message);
  }
};
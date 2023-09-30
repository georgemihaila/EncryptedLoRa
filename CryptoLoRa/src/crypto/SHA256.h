#pragma once

#include "Crypto.h"

namespace Crypto
{
  class SHA256
  {
  public:
    static String hash(String message);
    static String hmac(String message, String salt);
  };
};
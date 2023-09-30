#pragma once

#include "Crypto.h"

namespace Crypto
{
  class SHA256
  {
  private:
      static String _shorten_to_eight(String hash);
  public:
    static String hash_short(String message);
    static String hash(String message);
    static String hmac_short(String messsage, String salt);
    static String hmac(String message, String salt);
  };
};
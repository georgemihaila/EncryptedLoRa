#pragma once

#include "Crypto.h"
#include "mbedtls/base64.h"
#include "../logger/Logger.h"

#define KEY_SIZE 2048
#define EXPONENT 65537

namespace Crypto
{
  class RSA2048
  {
  public:
    static String sign(String message, String key);
    static bool verify(String message, String signature_base64, String pubkey);
    static String genPrivateKey();
    static String derivatePublicKey(const String &privateKey);
    static void test(Logger *logger);
  };
};
#ifndef CRYPTO_H
#define CRYPTO_H

#include <Arduino.h>
#include "mbedtls/md.h"
#include "mbedtls/pk.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "Cipher.h"
#include <iostream>
#include <string>
#include "Keys.h"

namespace Crypto
{
  // public:
  class RSA2048;
  class AES128;
  class SHA256;
  class RSAKeyPair;
};
#endif
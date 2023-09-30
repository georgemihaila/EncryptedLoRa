#pragma once

#include "Crypto.h"
using namespace Crypto;

namespace Crypto
{
  class RSAKeyPair
  {
  public:
    String privateKey;
    String publicKey;
    RSAKeyPair();

    RSAKeyPair(String privateKey, String publicKey);

    String sign(String message);

    bool verify(String message);
  };
};

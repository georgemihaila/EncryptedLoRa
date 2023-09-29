#pragma once

#include "Crypto.h"

namespace Crypto
{
  class RSAKeyPair
  {
  public:
    String privateKey;
    String publicKey;
    KeyPair()
    {
      privateKey = RSA2048::genPrivateKey();
      publicKey = RSA2048::derivatePublicKey(privateKey);
    }

    KeyPair(String privateKey, String publicKey)
    {
      this->privateKey = privateKey;
      this->publicKey = publicKey;
    }

    String sign(String message)
    {
      return RSA2048::sign(message, privateKey);
    }

    bool verify(String message)
    {
      return strcmp(RSA2048::verify(message, publicKey).c_str(), message.c_str()) == 0;
    }
  }
};

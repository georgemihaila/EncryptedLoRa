#pragma once

#include "RSAKeyPair.h"
#include "RSA2048.h"

namespace Crypto
{
  RSAKeyPair::RSAKeyPair()
  {
    this->privateKey = RSA2048::genPrivateKey();
    this->publicKey = RSA2048::derivatePublicKey(this->privateKey);
  }

  RSAKeyPair::RSAKeyPair(String privateKey, String publicKey)
  {
    this->privateKey = privateKey;
    this->publicKey = publicKey;
  }

  String RSAKeyPair::sign(String message)
  {
    return RSA2048::sign(message, this->privateKey);
  }

  bool RSAKeyPair::verify(String message)
  {
    return RSA2048::verify(message, sign(message), this->publicKey);
  }
};

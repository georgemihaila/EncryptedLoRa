#pragma once

#include "RSAKeyPair.h"
#include "RSA2048.h"

namespace Crypto
{
  RSAKeyPair::RSAKeyPair()
  {
    privateKey = RSA2048::genPrivateKey();
    publicKey = RSA2048::derivatePublicKey(privateKey);
  }

  RSAKeyPair::RSAKeyPair(String privateKey, String publicKey)
  {
    this->privateKey = privateKey;
    this->publicKey = publicKey;
  }

  String RSAKeyPair::sign(String message)
  {
    return RSA2048::sign(message, privateKey);
  }

  bool RSAKeyPair::verify(String message)
  {
    return strcmp(RSA2048::verify(message, publicKey).c_str(), message.c_str()) == 0;
  }
};

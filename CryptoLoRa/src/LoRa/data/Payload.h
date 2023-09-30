#pragma once

#include <Arduino.h>
#include "../../crypto/Crypto.h"
#include "../../crypto/RSAKeyPair.h"

namespace Data
{
  class Payload
  {
  private:
    RSAKeyPair *_key;

  public:
    String action;
    String method;
    String params;
    String getSignature();
    Payload(RSAKeyPair *_key);
    Payload(String action, String method, String params, RSAKeyPair *_key);
    Payload(String action, String method, RSAKeyPair *_key);
    Payload(String action, RSAKeyPair *_key);
    String toJSON();
    String toUnsignedJSON();
  };
};
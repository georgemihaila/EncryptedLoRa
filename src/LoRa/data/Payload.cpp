#pragma once

#include "Payload.h"

namespace Data
{
  Payload::Payload(RSAKeyPair *key) : action(""), method(""), params(""), _key(key)
  {
    // constructor body is empty as everything is initialized in the initializer list
  }

  Payload::Payload(String action, String method, String params, RSAKeyPair *key) : action(action), method(method), params(params), _key(key)
  {
    // constructor body is empty as everything is initialized in the initializer list
  }

  Payload::Payload(String action, String method, RSAKeyPair *key) : Payload(action, method, "", key)
  {
    // constructor body is empty as everything is initialized in the initializer list
  }

  Payload::Payload(String action, RSAKeyPair *key) : Payload(action, "", "", key)
  {
    // constructor body is empty as everything is initialized in the initializer list
  }

  String Payload::toJSON()
  {
    return "{\"action\":\"" + action + "\",\"method\":\"" + method + "\",\"params\":\"" + params + "\",\"signature\":\"" + getSignature() + "\"}";
  }

  String Payload::getSignature()
  {
    return _key->sign(this->toJSON());
  }
};

#pragma once

#include <Arduino.h>
#include "../../crypto/Crypto.h"
#include "../../crypto/RSAKeyPair.h"

namespace Data
{
  class Payload
  {
  public:
    String action;
    String method;
    String params;
    String getChecksum();
    Payload();
    Payload(String action, String method, String params);
    Payload(String action, String method);
    Payload(String action);
    String toJSON();
    String toUncheckedJSON() const;
    static Payload *fromCheckedJSON(String json);
  };
};
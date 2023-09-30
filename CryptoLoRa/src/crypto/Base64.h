#pragma once

#include <Arduino.h>
#include "mbedtls/base64.h"


namespace Crypto {
	class Base64 {
		public:
		static String encode(String input);
		static String decode(String input);
	};
};
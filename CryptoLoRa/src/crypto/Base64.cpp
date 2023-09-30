#pragma once

#include "Base64.h"

namespace Crypto {
	String Base64::encode(String input) {
		size_t inputLen = input.length();
		size_t olen;

		mbedtls_base64_encode(NULL, 0, &olen, (const unsigned char*)input.c_str(), inputLen);

		unsigned char output[olen];

		mbedtls_base64_encode(output, olen, &olen, (const unsigned char*)input.c_str(), inputLen);

		// Convert unsigned char array to String
		return String((char*)output);
	}

	String Base64::decode(String input) {
		size_t inputLen = input.length();
		size_t olen;

		mbedtls_base64_decode(NULL, 0, &olen, (const unsigned char*)input.c_str(), inputLen);

		unsigned char output[olen];

		if (mbedtls_base64_decode(output, olen, &olen, (const unsigned char*)input.c_str(), inputLen) != 0) {
			return "Decoding failed";
		}

		// Convert unsigned char array to String
		return String((char*)output);
	}
}
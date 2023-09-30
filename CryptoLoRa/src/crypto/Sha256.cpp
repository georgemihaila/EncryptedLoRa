#pragma once

#include "SHA256.h"
#include <cstddef>

namespace Crypto
{
    String SHA256::hash(String message)
    {
        mbedtls_md_context_t ctx;
        mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
        byte shaResult[32];

        mbedtls_md_init(&ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
        mbedtls_md_starts(&ctx);
        mbedtls_md_update(&ctx, (const unsigned char*)message.c_str(), message.length());
        mbedtls_md_finish(&ctx, shaResult);
        mbedtls_md_free(&ctx);

        String result = "";
        for (int i = 0; i < sizeof(shaResult); i++)
        {
            char str[3];
            sprintf(str, "%02x", (int)shaResult[i]);
            result += str;
        }

        return result;
    }

    String SHA256::hmac(String message, String salt)
    {
        mbedtls_md_context_t ctx;
        mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
        byte hmacResult[32];

        mbedtls_md_init(&ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1); // 1 for HMAC mode
        mbedtls_md_hmac_starts(&ctx, (const unsigned char*)salt.c_str(), salt.length());
        mbedtls_md_hmac_update(&ctx, (const unsigned char*)message.c_str(), message.length());
        mbedtls_md_hmac_finish(&ctx, hmacResult);
        mbedtls_md_free(&ctx);

        String result = "";
        for (int i = 0; i < sizeof(hmacResult); i++)
        {
            char str[3];
            sprintf(str, "%02x", (int)hmacResult[i]);
            result += str;
        }

        return result;
    }

    String SHA256::hmac_short(String message, String salt)
    {
        return _shorten_to_eight(hmac(message, salt));
    }

    String SHA256::hash_short(String message) 
    {
        return _shorten_to_eight(hash(message));
    }


    /* 
    XORs the 8 parts of the SHA-256 hash
    */
    String SHA256::_shorten_to_eight(String hash) 
    {
        if (hash.length() != 64) {
            return "Invalid hash length"; // SHA-256 hash should be 64 characters
        }

        uint32_t parts[8] = { 0 }; // 8 parts, each 4 bytes (uint32_t)
        uint32_t result = 0;

        for (int i = 0; i < 8; ++i) 
        {
            String part = hash.substring(i * 8, (i + 1) * 8);
            parts[i] = strtoul(part.c_str(), NULL, 16);
            result ^= parts[i];
        }

        char resultStr[9]; // 8 bytes + null terminator
        snprintf(resultStr, sizeof(resultStr), "%08X", result);

        return String(resultStr);
    }
}

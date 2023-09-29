#pragma once

#include "SHA256.h"

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
    mbedtls_md_update(&ctx, (const unsigned char *)message.c_str(), message.length());
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
    mbedtls_md_hmac_starts(&ctx, (const unsigned char *)salt.c_str(), salt.length());
    mbedtls_md_hmac_update(&ctx, (const unsigned char *)message.c_str(), message.length());
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
}

#pragma once

#include "RSA2048.h"

namespace Crypto
{
  String RSA2048::sign(String message, String key)
  {
    mbedtls_pk_context pk;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_pk_init(&pk);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    // Initialize RNG
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)PSK, 0);

    // Parse the private key
    if (mbedtls_pk_parse_key(&pk, reinterpret_cast<const unsigned char *>(key.c_str()), key.length() + 1, NULL, 0) != 0)
    {
      return "Failed to parse key";
    }

    // Compute hash of message
    unsigned char hash[32];
    if (mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), reinterpret_cast<const unsigned char *>(message.c_str()), message.length(), hash) != 0)
    {
      return "Failed to compute hash";
    }

    // Sign hash
    unsigned char buf[MBEDTLS_MPI_MAX_SIZE];
    size_t olen = 0;

    if (mbedtls_pk_sign(&pk, MBEDTLS_MD_SHA256, hash, 0, buf, &olen, mbedtls_ctr_drbg_random, &ctr_drbg) != 0)
    {
      return "Failed to sign";
    }

    // Convert to Base64
    // Convert to Base64
    size_t base64_len;
    size_t base64_max_len = 4 * ((olen + 2) / 3) + 1; // +1 for null-terminator if you plan to use it as a string
    unsigned char *base64_output = new unsigned char[base64_max_len];
    // Clear the buffer to zero (optional)
    memset(base64_output, 0, base64_max_len);

    int ret = mbedtls_base64_encode(base64_output, base64_max_len, &base64_len, buf, olen);
    if (ret != 0)
    {
      delete[] base64_output;
      return "Failed to encode to Base64";
    }
    // Cleanup
    mbedtls_pk_free(&pk);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    delete[] base64_output;
    // Return the Base64 encoded signed message
    return String(reinterpret_cast<char *>(base64_output), base64_len);
  }

  bool RSA2048::verify(String message, String signature_base64, String pubkey)
  {
    mbedtls_pk_context pk;
    mbedtls_pk_init(&pk);

    // Parse the public key
    if (mbedtls_pk_parse_public_key(&pk, reinterpret_cast<const unsigned char *>(pubkey.c_str()), pubkey.length() + 1) != 0)
    {
      return false;
    }

    // Compute hash of message
    unsigned char hash[32];
    if (mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), reinterpret_cast<const unsigned char *>(message.c_str()), message.length(), hash) != 0)
    {
      return false;
    }

    // Decode Base64 signature
    size_t sig_len;
    unsigned char sig[MBEDTLS_MPI_MAX_SIZE];

    if (mbedtls_base64_decode(sig, sizeof(sig), &sig_len, reinterpret_cast<const unsigned char *>(signature_base64.c_str()), signature_base64.length()) != 0)
    {
      return false;
    }

    // Verify the signature
    if (mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, hash, 0, sig, sig_len) != 0)
    {
      return false;
    }

    mbedtls_pk_free(&pk);

    return true;
  }

  String RSA2048::genPrivateKey()
  {
    mbedtls_pk_context key;
    mbedtls_pk_init(&key);

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                          (const unsigned char *)PSK, strlen(PSK));

    mbedtls_pk_setup(&key, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA));
    mbedtls_rsa_gen_key(mbedtls_pk_rsa(key), mbedtls_ctr_drbg_random, &ctr_drbg,
                        KEY_SIZE, EXPONENT);

    // Allocate memory dynamically for the buffer
    char *buf = new char[KEY_SIZE * 2];

    // Check if memory allocation was successful
    if (buf == nullptr)
    {
      // Handle error
      Serial.println("Memory allocation failed!");
      return "Memory allocation failed!";
    }

    size_t len = 0;
    mbedtls_pk_write_key_pem(&key, (unsigned char *)buf, KEY_SIZE * 2);
    len = strlen(buf); // Update the length

    String result(buf, len); // Use the constructor that takes a C-string and length

    // Don't forget to free the dynamically allocated memory!
    delete[] buf;

    mbedtls_pk_free(&key);
    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    return result;
  }

  String RSA2048::derivatePublicKey(const String &privateKey)
  {
    mbedtls_pk_context key;
    mbedtls_pk_init(&key);

    // Parse private key from string
    if (mbedtls_pk_parse_key(&key, (const unsigned char *)privateKey.c_str(), privateKey.length() + 1, nullptr, 0) != 0)
    {
      mbedtls_pk_free(&key);
      return "Invalid private key.";
    }

    // Extract public key from the private key
    char buf[KEY_SIZE * 2];
    size_t len = 0;

    // Get the key type
    mbedtls_pk_type_t keyType = mbedtls_pk_get_type(&key);

    // Check if the key type is private (MBEDTLS_PK_RSA or MBEDTLS_PK_RSA_ALT)
    if (keyType == MBEDTLS_PK_RSA || keyType == MBEDTLS_PK_RSA_ALT)
    {
      // For RSA, we parse the public key separately
      mbedtls_pk_context pubKey;
      mbedtls_pk_init(&pubKey);

      if (mbedtls_pk_parse_public_key(&pubKey, (const unsigned char *)privateKey.c_str(), privateKey.length() + 1) == 0)
      {
        // Write the public key to PEM format
        mbedtls_pk_write_pubkey_pem(&key, (unsigned char *)buf, sizeof(buf));
        mbedtls_pk_free(&pubKey);
      }
    }
    else
    {
      // For other key types, directly write the public key to PEM format
      mbedtls_pk_write_pubkey_pem(&key, (unsigned char *)buf, sizeof(buf));
    }

    String result(buf, len);

    mbedtls_pk_free(&key);

    return result;
  }

  void RSA2048::test(Logger *logger)
  {
    if (logger == nullptr)
      return;

    logger->info("Testing RSA2048...");
    logger->info("Generating 2048-bit RSA private key...");
    String pk = RSA2048::genPrivateKey();
    logger->info("Derivating public key...");
    String pubk = RSA2048::derivatePublicKey(pk);
    logger->info("Testing signature...");
    String testString = "abc123";
    String signedString = RSA2048::sign(testString, pk);
    if (!RSA2048::verify(testString, signedString, pubk))
    {
      logger->info("RSA signature verification failed");
    }
    else
    {
      logger->info("RSA ok");
    }
    logger->info("Testing invalid signature...");
    String invalidString = "abc1234";
    if (RSA2048::verify(invalidString, signedString, pubk))
    {
      logger->info("RSA invalid signature verification failed");
    }
    else
    {
      logger->info("RSA invalid signature test ok");
    }
  }
};
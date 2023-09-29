#pragma once

#include "RSA2048.h"

namespace Crypto
{
  String RSA2048::sign(String message, String key)
  {
    mbedtls_pk_context pk;
    mbedtls_pk_init(&pk);
    mbedtls_pk_parse_key(&pk, (const unsigned char *)key.c_str(), key.length() + 1, NULL, 0);
    mbedtls_pk_sign(&pk, MBEDTLS_MD_SHA256, (const unsigned char *)message.c_str(), message.length(), (unsigned char *)message.c_str(), (size_t *)message.length(), NULL, NULL);
    mbedtls_pk_free(&pk);
    return message;
  }

  String RSA2048::verify(String message, String pubkey)
  {
    mbedtls_pk_context pk;
    mbedtls_pk_init(&pk);
    mbedtls_pk_parse_public_key(&pk, (const unsigned char *)pubkey.c_str(), pubkey.length() + 1);
    mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, (const unsigned char *)message.c_str(), message.length(), (unsigned char *)message.c_str(), message.length());
    mbedtls_pk_free(&pk);
    return message;
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
    String verification = RSA2048::verify(signedString, pubk);
    if (strcmp(testString.c_str(), verification.c_str()) != 0)
    {
      logger->info("RSA signature verification failed");
    }
    else
    {
      logger->info("RSA ok");
    }
    logger->info("Testing invalid signature...");
    String invalidString = "abc1234";
    if (strcmp(testString.c_str(), RSA2048::verify(RSA2048::sign(invalidString, pk), pubk).c_str()) == 0)
    {
      logger->info("RSA invalid signature verification failed");
    }
    else
    {
      logger->info("RSA invalid signature test ok");
    }
  }
};
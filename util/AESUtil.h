#ifndef AESUTIL_H
#define AESUTIL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <iomanip>
#include "types.h"

class AESUtil
{
    public:
        AESUtil(uint8_t *kd, uint32_t *s);
        AESUtil();
        virtual ~AESUtil();
        uint8_t *getkeyData();
        uint8_t *encrypt(uint8_t *plainText, int *len);
        uint8_t *decrypt(uint8_t *cipherText, int *len);

    protected:

    private:
        uint8_t *keyData;
        int keyDataLen;
        EVP_CIPHER_CTX en, de;
        uint32_t *salt ;
};

#endif // AESUTIL_H

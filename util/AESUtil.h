/*
Copyright (C) 2011 Iraide Diaz (Sharem)
Copyright (C) 2011 Xabier Larrakoetxea (slok)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AESUTIL_H
#define AESUTIL_H

#include <string.h>
#include <iostream>
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

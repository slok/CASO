#include "AESUtil.h"

AESUtil::AESUtil()
{
    //ctor
}

AESUtil::~AESUtil()
{
    EVP_CIPHER_CTX_cleanup(&this->en);
    EVP_CIPHER_CTX_cleanup(&this->de);
}
uint8_t *AESUtil::getkeyData()
{
    return this->keyData;
}

 /** @brief (one liner)
  *
  * (documentation goes here)
  */
AESUtil::AESUtil(uint8_t* kd, uint32_t* s)
{
    this->keyData = kd;
    this->keyDataLen = strlen((const char *)kd);
    this->salt = s;

    int i, nrounds = 5;
    uint8_t key[32], iv[32];

    /*
    * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
    * nrounds is the number of times the we hash the material. More rounds are more secure but
    * slower.
    */
    i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), (uint8_t *)salt, this->keyData, this->keyDataLen, nrounds, key, iv);
    if (i != 32)
    {
        printf("Key size is %d bits - should be 256 bits\n", i);

    }

    EVP_CIPHER_CTX_init(&this->en);
    EVP_EncryptInit_ex(&this->en, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_CIPHER_CTX_init(&this->de);
    EVP_DecryptInit_ex(&this->de, EVP_aes_256_cbc(), NULL, key, iv);

}

uint8_t *AESUtil::encrypt(uint8_t *plainText, int *len)
{
    uint8_t *cipherText = new uint8_t();

    /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
    int c_len = *len + AES_BLOCK_SIZE, f_len = 0;

    /* allows reusing of 'e' for multiple encryption cycles */
    EVP_EncryptInit_ex(&this->en, NULL, NULL, NULL, NULL);

    /* update ciphertext, c_len is filled with the length of ciphertext generated,
    *len is the size of plaintext in bytes */
    EVP_EncryptUpdate(&this->en, cipherText, &c_len, plainText, *len);

    /* update ciphertext with the final remaining bytes */
    EVP_EncryptFinal_ex(&this->en, cipherText+c_len, &f_len);

    *len = c_len + f_len;

    return cipherText;
}

uint8_t *AESUtil::decrypt(uint8_t *cipherText, int *len)
{
    uint8_t *plainText = new uint8_t();

    /* plaintext will always be equal to or lesser than length of ciphertext*/
    int p_len = *len, f_len = 0;

    EVP_DecryptInit_ex(&this->de, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&this->de, plainText, &p_len, cipherText, *len);
    EVP_DecryptFinal_ex(&this->de, plainText+p_len, &f_len);

    *len = p_len + f_len;

    return plainText;
}

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
    /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
    *len = *len +1;
    int c_len = *len + AES_BLOCK_SIZE, f_len = 0;

    uint8_t *cipherText = new uint8_t[c_len];
    //uint8_t *cipherText = (uint8_t *)malloc(c_len);

    /*std::cout << "clen:" << c_len << std::endl;
    std::cout << " len:" << *len << std::endl;
    std::cout << "flen:" << f_len << std::endl;*/

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
    /* plaintext will always be equal to or lesser than length of ciphertext*/
    int p_len = *len, f_len = 0;
    uint8_t *plainText = new uint8_t[p_len];
    //uint8_t *plainText = (uint8_t *)malloc(p_len);

    /*std::cout << " len:" << *len << std::endl;
    std::cout << "plen:" << p_len << std::endl;
    std::cout << "flen:" << f_len << std::endl;*/

    EVP_DecryptInit_ex(&this->de, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(&this->de, plainText, &p_len, cipherText, *len);

    EVP_DecryptFinal_ex(&this->de, plainText+p_len, &f_len);

    *len = p_len + f_len;

    return plainText;
}

/*
using namespace std;

int main()
{

    //cout << "Hello world!" << endl;
    uint32_t salt[] = {12345, 54321};
    string tmp;
    uint8_t * testText = NULL;
    int len;
    uint8_t *cifrado = NULL;
    uint8_t *descifrado = NULL;

    cout << "#############################################\n";
    cout << "#Inserta un palabra a encriptar en AES 256: #\n";
    cout << "#############################################\n";
    cin >> tmp;
    //tmp = "abcd";
    testText = (uint8_t *) tmp.c_str();
    //len = strlen((const char *)testText)+1;
    len = tmp.size()+1;

    AESUtil prueba((uint8_t *)"key secreta", salt);
    cifrado = prueba.encrypt(testText, &len);
    descifrado = prueba.decrypt(cifrado, &len);

    cout << "************Resultados**********************\n";
    cout << "key: " << prueba.getkeyData() << endl;
    cout << "Text: " << testText << endl;
    cout << "Crypted text(ASCII): "<< cifrado << endl;
    //cout << "¿Crypted text(Hexadecimal)?: "<< (int*)cifrado << endl;
    cout << "Decrypted text: "<< descifrado << endl;
    cout << "********************************************\n";

    delete cifrado;
    delete descifrado;

    return 0;
}
*/

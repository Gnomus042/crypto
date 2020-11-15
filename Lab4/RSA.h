//
// Created by anast on 11/14/2020.
//

#ifndef CRYPTO_RSA_H
#define CRYPTO_RSA_H

#include "../BigInt.h"
#include "MillerRabin.h"

struct Key {
    BigInt mod;
    BigInt val;
};

class RSA {
public:
    pair<Key, Key> keygen(int prime_size);
    vector<uint8_t> encrypt(const vector<uint8_t>& m, const Key& pk);
    vector<uint8_t> decrypt(const vector<uint8_t>& c, const Key& sk);
    vector<uint8_t> encrypt_oaep(const vector<uint8_t>& m, const Key& pk, int block_size);
    vector<uint8_t> decrypt_oaep(const vector<uint8_t>& c, const Key& sk, int block_size);
private:
    BigInt get_prime(int prime_size);
};


#endif //CRYPTO_RSA_H

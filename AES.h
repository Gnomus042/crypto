//
// Created by anast on 9/26/2020.
//

#ifndef CRYPTO_AES_H
#define CRYPTO_AES_H

#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>
#include "helper.h"

using namespace std;

class AES {
public:
    explicit AES(const vector<uint8_t>& key);
    vector<uint8_t> encrypt(vector<uint8_t> data);
    vector<uint8_t> decrypt(vector<uint8_t> data);
private:
    void generate_sbox();
    void generate_inv_sbox();
    void make_key_schedule(const Block &key, int blocks_count);
    Block encrypt_block(const Block &data);
    Block decrypt_block(const Block &data);
    int blocks_count;
    vector<Byte> sbox;
    vector<Byte> inv_sbox;
    Block rcon;
    vector<Block> key_schedule;
};


#endif //CRYPTO_AES_H

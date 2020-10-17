//
// Created by anast on 9/26/2020.
//

#ifndef CRYPTO_AES_H
#define CRYPTO_AES_H

#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>
#include "../helper.h"

using namespace std;

class AES {
public:
    enum class Mode{
        ECB, CBC, CFB, OFB, CTR
    };
    explicit AES(const vector<uint8_t>& key, const vector<uint8_t>& in_vector);
    vector<uint8_t> encrypt(vector<uint8_t> data, Mode mode);
    vector<uint8_t> decrypt(vector<uint8_t> data, Mode mode);
private:
    void generate_sbox();
    void generate_inv_sbox();
    void make_key_schedule(const Block &key, int blocks_count);

    vector<uint8_t> encrypt_ecb(const vector<uint8_t>& data, bool is_encryption);
    vector<uint8_t> encrypt_cbc(const vector<uint8_t>& data, bool is_encryption);
    vector<uint8_t> encrypt_cfb(vector<uint8_t> data, bool is_encryption);
    vector<uint8_t> encrypt_ofb(vector<uint8_t> data, bool is_encryption);
    vector<uint8_t> encrypt_ctr(vector<uint8_t> data, bool is_encryption);

    Block encrypt_block(const Block &data);
    Block decrypt_block(const Block &data);
    int blocks_count;
    vector<Byte> sbox;
    vector<Byte> inv_sbox;
    Block rcon;
    vector<Block> key_schedule;
    vector<uint8_t> in_vector;
};


#endif //CRYPTO_AES_H

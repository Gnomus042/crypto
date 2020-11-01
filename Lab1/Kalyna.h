//
// Created by anast on 10/4/2020.
//

#ifndef CRYPTO_KALYNA_H
#define CRYPTO_KALYNA_H

#include <vector>
#include <algorithm>
#include <iostream>
#include "../helper.h"
#include "../kalyna_tables.h"

using namespace std;

class Kalyna {
public:
    explicit Kalyna(const vector<uint8_t> &key, int key_size, int data_size);

    vector<uint8_t> encrypt(vector<uint8_t> data);

    vector<uint8_t> decrypt(vector<uint8_t> data);

private:
    Block encrypt_block(const Block &data);

    Block decrypt_block(const Block &data);

    Block make_intermediate_key(const Block &key);

    void make_key_schedule(Block &key, int blocks_count);
    Block even_round_key(Block& key, Block& ikey, int iter, Block& tmv);
    vector<Block> key_schedule;

    void shift_rows(Block &block);
    void inv_shift_rows(Block &block);
    vector<Byte> mult;
    vector<Byte> inv_mult;
    vector<vector<Byte>> sboxes;
    vector<vector<Byte>> inv_sboxes;
    int key_size;
    int block_size;
    int columns;
    int rounds;
};

#endif //CRYPTO_KALYNA_H

//
// Created by anast on 10/31/2020.
//

#include "Kupyna.h"

Kupyna::Kupyna(int mode) {
    if (mode < 8 || mode > 512) {
        throw "Invalid mode provided";
    }
    this->mode = mode;
    this->block_size = this->mode <= 256 ? 64 : 128;
    this->sboxes = {sbox0, sbox1, sbox2, sbox3};
}

vector<uint8_t> Kupyna::padding(const vector<uint8_t>& data) {
    vector<uint8_t> m = data;
    m.push_back(128);
    while (m.size()%block_size != block_size-12) {
        m.push_back(0);
    }
    vector<uint8_t> temp = u64to8((uint64_t)(data.size()*8));
    m.insert(m.end(),temp.begin(), temp.end());
    m.push_back(0);
    m.push_back(0);
    m.push_back(0);
    m.push_back(0);
    return m;
}

vector<uint8_t> Kupyna::hash(const vector<uint8_t>& data) {
    vector<Block> h;
    vector<uint8_t> m = padding(data);
    Block h0(8, mode <= 256 ? 8 : 16);
    if (block_size == 64) h0.data[0][0] = {64};
    else h0.data[0][0] = {128};
    h.push_back(h0);

    for (int i = 0; i < m.size()/block_size; i++) {
        Block block (m.data()+i*block_size, 8, mode <= 256 ? 8 : 16);
        Block t1 = block + h[i];
        t_xor(t1);
        Block t2 = block;
        t_plus(t2);
        h.push_back(t1+t2+h[i]);
    }

    Block hk (h.back());
    t_xor(hk);
    vector<uint8_t> hash = (hk + h.back()).get_data();
    vector<uint8_t> res (hash.end()-this->mode/8, hash.end());
    return res;
}

void Kupyna::t_xor(Block &data) {
    int t = mode <= 256 ? 10 : 14;
    for (int i = 0; i < t; i++) {
        iteration_constant_addition_k(data, i);
        bijective_mapping(data);
        internal_state_permutation(data);
        linear_transformation(data);
    }
}

void Kupyna::t_plus(Block& data) {
    int t = block_size == 64 ? 10 : 14;
    for (int i = 0; i < t; i++) {
        iteration_constant_addition_n(data, i);
        bijective_mapping(data);
        internal_state_permutation(data);
        linear_transformation(data);
    }
}



void Kupyna::internal_state_permutation(Block& block) {
   for (int i = 0; i <= 6; i++) {
        block.rotate_row(i, i);
    }
    block.rotate_row(7, block_size == 64 ? 7 : 11);
}

void Kupyna::iteration_constant_addition_k(Block &block, int iteration) {
    for (uint8_t j = 0; j < block.y; j++) {
        Byte temp_b = {(uint8_t)(j<<4)};
        Byte iteration_b = {(uint8_t)iteration};
        block.data[0][j] = block.data[0][j] + temp_b + iteration_b;
    }
}

void Kupyna::iteration_constant_addition_n(Block &block, int iteration) {
    for (int j = 0; j < block.y; j++) {
        uint64_t val = (0x00F0F0F0F0F0F0F3ULL ^
                   ((uint64_t)(((block.y - j - 1) * 0x10ULL) ^ iteration) << (7 * 8)));
        vector<uint8_t> temp = u64to8(val);
        int rem = 0;
        for (int i = 0; i < block.x; i++) {
            int q = ((int)block.data[i][j].data + (int)temp[i] + rem);
            rem = q > 255? 1 : 0;
            block.data[i][j] = {(uint8_t)q};
        }
    }
}

void Kupyna::linear_transformation(Block &block) {
    mix_columns(block, {{0x01}, {0x01}, {0x05}, {0x01},
                        {0x08}, {0x06}, {0x07}, {0x04}});
}

void Kupyna::bijective_mapping(Block &block) {
    sub_bytes(block, this->sboxes);
}



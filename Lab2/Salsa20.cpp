//
// Created by anast on 10/11/2020.
//

#include "Salsa20.h"

Salsa20::Salsa20(const vector<uint8_t> &key) {
    for (int i = 0; i < key.size() / 4; i++)
        this->key.push_back(u8to32(key.data() + 4 * i));
}

vector<uint8_t> Salsa20::crypt(const vector<uint8_t> &in) {
    vector<uint8_t> res (in.begin(), in.end());
    vector<uint32_t> keystream = make_block({0, 0}, {0, 0});
    vector<uint8_t> curr_word;
    for (int i = 0; i < res.size(); i++) {
        if ((i+1)%64 == 0) keystream = make_block({0, (uint32_t)(i/64)}, {0, (uint32_t)(i/64)});
        if (i%4 == 0) curr_word = u32to8(keystream[i%16]);
        res[i] ^= curr_word[i%4];
    }
    return res;
}

uint32_t Salsa20::left_rotate(uint32_t val, int d) {
    return (val << d) | (val >> (32 - d));
}

vector<uint32_t> Salsa20::quaterround(const vector<uint32_t> &word) {
    assert(word.size() == 4);
    vector<uint32_t> new_word(4);
    new_word[1] = word[1] + Salsa20::left_rotate(word[0] + word[3], 7);
    new_word[2] = word[2] + Salsa20::left_rotate(new_word[1] + word[0], 9);
    new_word[3] = word[3] + Salsa20::left_rotate(new_word[2] + new_word[1], 13);
    new_word[0] = word[0] + Salsa20::left_rotate(new_word[3] + new_word[2], 18);
    return new_word;
}

vector<uint32_t> Salsa20::rowround(const vector<uint32_t> &table) {
    vector<uint32_t> res(16);
    for (int i = 0; i < 4; i++) {
        vector<uint32_t> word(4);
        for (int j = 0; j < 4; j++) word[i] = table[i * 4 + j];
        vector<uint32_t> new_word = this->quaterround(word);
        for (int j = 0; j < 4; j++) res[i * 4 + j] = new_word[j];
    }
    return res;
}

vector<uint32_t> Salsa20::columnround(const vector<uint32_t> &table) {
    vector<uint32_t> res(16);
    for (int i = 0; i < 4; i++) {
        vector<uint32_t> word(4);
        for (int j = 0; j < 4; j++) word[i] = table[j * 4 + i];
        vector<uint32_t> new_word = this->quaterround(word);
        for (int j = 0; j < 4; j++) res[j * 4 + i] = new_word[j];
    }
    return res;
}

vector<uint32_t> Salsa20::doubleround(const vector<uint32_t> &table) {
    return rowround(columnround(table));
}

void Salsa20::hash(vector<uint32_t> &stream) {
    vector<uint32_t> hashed(stream.begin(), stream.end());
    for(int i = 0; i < 10; i++) {
        hashed = doubleround(hashed);
    }
    for(int i = 0; i < stream.size(); i++) stream[i] += hashed[i];
}

vector<uint32_t> Salsa20::make_block(const vector<uint32_t>& nonce, const vector<uint32_t>& block) {
    vector<uint32_t> keystream = {
            0x61707865, key[0], key[1], key[2],
            key[3], 0x3320646e, nonce[0], nonce[1],
            block[0], block[1], 0x79622d32, key[4],
            key[5], key[6], key[7], 0x6b206574
    };
    hash(keystream);
    return keystream;
}

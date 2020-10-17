//
// Created by anast on 10/11/2020.
//

#ifndef CRYPTO_SALSA20_H
#define CRYPTO_SALSA20_H

#include <vector>
#include <cassert>

#include "../helper.h"

class Salsa20 {
public:
    Salsa20(const vector<uint8_t>& key);
    vector<uint8_t> crypt(const vector<uint8_t>& data);
private:
    void hash(vector<uint32_t>& data);
    vector<uint32_t> make_block(const vector<uint32_t>& nonce, const vector<uint32_t>& block);
    vector<uint32_t> quaterround(const vector<uint32_t> &word);
    vector<uint32_t> rowround(const vector<uint32_t> &table);
    vector<uint32_t> columnround(const vector<uint32_t> &table);
    vector<uint32_t> doubleround(const vector<uint32_t> &table);
    static uint32_t left_rotate(uint32_t val, int d);
    vector<uint32_t> key;
};


#endif //CRYPTO_SALSA20_H

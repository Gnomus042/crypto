//
// Created by anast on 10/3/2020.
//

#ifndef CRYPTO_HELPER_H
#define CRYPTO_HELPER_H

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

struct Byte {
    uint8_t data;
    Byte operator+(const Byte &other);
    Byte operator*(const Byte &other);
    static Byte pow(Byte val, int power);
    static Byte invert(const Byte &val);
};

struct Block {
    vector<vector<Byte>> data;
    int x;
    int y;
    explicit Block();
    Block(const uint8_t* data, int x, int y);
    Block(int x, int y);
    void copyFrom(const Block& other);
    vector<uint8_t> get_data();
    void print() const;
};

void sub_bytes(Block& block, const vector<Byte> &substitution_box);
void shift_rows(Block& block);
void inv_shift_rows(Block& block);
void mix_columns(Block& block, const vector<Byte>& mult);
void add_round_key(Block& block, const Block& key);


#endif //CRYPTO_HELPER_H

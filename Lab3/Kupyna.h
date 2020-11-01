//
// Created by anast on 10/31/2020.
//

#ifndef CRYPTO_KUPYNA_H
#define CRYPTO_KUPYNA_H

#include "../helper.h"
#include "../kalyna_tables.h"
#include <bitset>


class Kupyna {
public:
    Kupyna(int mode);
    vector<uint8_t> hash(const vector<uint8_t>& data);
private:
    vector<uint8_t> padding(const vector<uint8_t>& data);
    void internal_state_permutation(Block& block);
    void iteration_constant_addition_k(Block& block, int iteration);
    void iteration_constant_addition_n(Block& block, int iteration);
    void linear_transformation(Block& block);
    void bijective_mapping(Block& block);
    void t_plus(Block& data);
    void t_xor(Block& data);
    int mode;
    int block_size;
    vector<vector<Byte>> sboxes;
};


#endif //CRYPTO_KUPYNA_H

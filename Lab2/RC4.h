//
// Created by anast on 10/10/2020.
//

#ifndef CRYPTO_RC4_H
#define CRYPTO_RC4_H

#include <vector>
#include "../helper.h"

class RC4 {
public:
    RC4(vector<uint8_t> key);
    vector<uint8_t> encrypt(const vector<uint8_t>& data);
    vector<uint8_t> decrypt(const vector<uint8_t>& data);
private:
    void make_key_schedule(Block& key);
    void pseudo_random_generation(Block& data);
    Block key_block;
    Block key_schedule;
};


#endif //CRYPTO_RC4_H

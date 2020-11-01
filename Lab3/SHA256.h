//
// Created by anast on 10/31/2020.
//

#ifndef CRYPTO_SHA256_H
#define CRYPTO_SHA256_H

#include "../helper.h"

class SHA256 {
public:
    static vector<uint8_t> hash(const vector<uint8_t>& message);
    static vector<uint32_t> H;
    static vector<uint32_t> K;
};


#endif //CRYPTO_SHA256_H

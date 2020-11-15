//
// Created by anast on 11/14/2020.
//

#ifndef CRYPTO_MILLERRABIN_H
#define CRYPTO_MILLERRABIN_H

#include "../BigInt.h"

class MillerRabin {
public:
    static bool test(const BigInt& n, int rounds);
};


#endif //CRYPTO_MILLERRABIN_H

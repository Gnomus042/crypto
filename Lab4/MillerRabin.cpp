//
// Created by anast on 11/14/2020.
//

#include "MillerRabin.h"

bool MillerRabin::test(const BigInt &n, int rounds) {
    if (n % 2 == 0)
        return false;
    int s = 0;
    BigInt t = n - 1;
    while (t % 2 == 0) {
        s += 1;
        t /= 2;
    }
    for (int i = 0; i < rounds; i++) {
        BigInt a = BigInt::rand(2, n - 2);
        BigInt x = BigInt::mpow(a, t, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int j = 0; j < s - 1; j++) {
            x = BigInt::mpow(x, 2, n);
            if (x == 1) return false;
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

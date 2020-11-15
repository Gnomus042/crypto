//
// Created by anast on 11/14/2020.
//

#ifndef CRYPTO_BIGINT_H
#define CRYPTO_BIGINT_H

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include "helper.h"

using namespace std;

const uint64_t BASE = 1ull << 32u;

class BigInt {
public:
    BigInt();

    BigInt(long long num);

    BigInt(vector<uint8_t> data);

    bool operator==(const BigInt &other) const;

    bool operator>(const BigInt &other) const;

    bool operator<(const BigInt &other) const;

    bool operator!=(const BigInt &other) const;

    BigInt operator+(const BigInt &other) const;

    BigInt operator-(const BigInt &other) const;

    BigInt operator/(const BigInt &other) const;

    BigInt operator*(const BigInt &other) const;

    BigInt operator%(const BigInt &other) const;

    BigInt &operator+=(const BigInt &other);

    BigInt &operator-=(const BigInt &other);

    BigInt &operator*=(const BigInt &other);

    BigInt &operator/=(const BigInt &other);

    static BigInt rand(const BigInt &from, const BigInt &to);

    static BigInt pow(const BigInt &n, const BigInt &p);

    static BigInt mpow(const BigInt &n, const BigInt &p, const BigInt &m);

    static BigInt gcd(BigInt first, BigInt second);

    static BigInt gcdExtended(BigInt a, BigInt b, BigInt& x, BigInt& y, bool&, bool&);

    static BigInt inverseModulo(const BigInt & a, const BigInt & m);

    vector<uint8_t> tou8();

    friend ostream operator<<(const ostream &o, const BigInt &other);

public:
    pair<BigInt, BigInt> DivMod(BigInt other) const;

    uint64_t GetTop2() const;

    vector<uint64_t> number;
};


#endif //CRYPTO_BIGINT_H

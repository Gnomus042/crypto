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

const uint64_t BASE_BITS = 32u;
const uint64_t BASE = 1ull << BASE_BITS;

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

    BigInt operator^(const BigInt &other) const;

    BigInt operator&(const BigInt &other) const;

    BigInt operator>>(uint64_t bits) const;

    BigInt operator<<(uint64_t bits) const;

    BigInt &operator+=(const BigInt &other);

    BigInt &operator-=(const BigInt &other);

    BigInt &operator*=(const BigInt &other);

    BigInt &operator/=(const BigInt &other);

    static BigInt rand(const BigInt &from, const BigInt &to);

    static BigInt pow(const BigInt &n, const BigInt &p);

    static BigInt mpow(BigInt n, BigInt p, const BigInt &m);

    static BigInt gcd(BigInt first, BigInt second);

    static BigInt gcdExtended(BigInt a, BigInt b, BigInt& x, BigInt& y, bool&, bool&);

    static BigInt inverseModulo(const BigInt & a, const BigInt & m);

    static BigInt fromHex(const std::string & hex);

    uint64_t bit_size() const;

    vector<uint8_t> tou8();

    friend ostream operator<<(const ostream &o, const BigInt &other);

    bool isOdd() const;

    bool isZero() const;

    uint64_t lowestDigit() const;

    void makePoly(bool p = true);

public:
    pair<BigInt, BigInt> DivMod(BigInt other) const;

    uint64_t GetTop2() const;

    uint64_t GetTop() const;

    void truncate();

    vector<uint64_t> number;

    bool is_poly = false;
};


#endif //CRYPTO_BIGINT_H

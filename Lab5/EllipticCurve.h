//
// Created by anast on 11/26/2020.
//

#ifndef CRYPTO_ELLIPTICCURVE_H
#define CRYPTO_ELLIPTICCURVE_H

#include "../BigInt.h"

using namespace std;

struct EllipticPoint {
    BigInt x;
    BigInt y;
    bool is_zero = true;

    EllipticPoint() = default;

    EllipticPoint(const BigInt &a, const BigInt &b);

    bool operator==(const EllipticPoint &other) const {
        return x == other.x && y == other.y && is_zero == other.is_zero;
    }

    bool operator!=(const EllipticPoint &other) const {
        return !(*this == other);
    }
};

class EllipticCurve {
public:
    EllipticCurve(BigInt a, BigInt b, int m, const vector<int> &poly_coefs, const BigInt &x, const BigInt &y,
                  const BigInt &n);

    EllipticPoint get_g() const;

    BigInt get_n() const;

    EllipticPoint create_point() const;

    bool check_point(const EllipticPoint &p) const;

    EllipticPoint add_points(const EllipticPoint &a, const EllipticPoint &b) const;

    EllipticPoint double_point(const EllipticPoint &a) const;

    EllipticPoint mul_point(const EllipticPoint &a, BigInt k) const;

private:
    BigInt gf_add(const BigInt &x, const BigInt &y) const;

    BigInt gf_mul(const BigInt &x, const BigInt &y) const;

    BigInt gf_pow(const BigInt &x, BigInt y) const;

    BigInt gf_inv(const BigInt &x) const;

    EllipticPoint g;
    BigInt n;
    BigInt A, B;
    BigInt poly;
    int m;
    BigInt r1, r2;
};

struct Signature {
    BigInt r;
    BigInt s;
};

struct PublicKey {
    EllipticPoint Q;
};

struct PrivateKey {
    BigInt x;
};

namespace EllipticCurveSignature {
    std::pair<PrivateKey, PublicKey> GenerateKeys(const EllipticCurve &curve);

    Signature Sign(const BigInt &message, const PrivateKey &key, const EllipticCurve &curve);

    bool CheckSign(const BigInt &message, const Signature &sign, const PublicKey &key, const EllipticCurve &curve);
};


BigInt gcd_extended(BigInt a, BigInt b, BigInt &x, BigInt &y);


#endif //CRYPTO_ELLIPTICCURVE_H

//
// Created by anast on 11/26/2020.
//

#include "EllipticCurve.h"

#include <utility>

EllipticCurve::EllipticCurve(BigInt a, BigInt b, int m, const vector<int> &poly_coefs, const BigInt & x, const BigInt & y, const BigInt & n)
: A(move(a)), B(move(b)), m(m),g(EllipticPoint(x, y)), n(n) {
    for (uint64_t p : poly_coefs) {
        poly = poly ^ (BigInt(1) << p);
    }
    r1 = BigInt(1) << m;
    r2 = r1 - 1;
}

EllipticPoint EllipticCurve::get_g() const {
    return g;
}

EllipticPoint EllipticCurve::create_point() const {
    // беремо точку-генератор та домножаємо
    return mul_point(get_g(), BigInt::rand(0, BigInt(1) << 32));
}

BigInt EllipticCurve::gf_add(const BigInt &x, const BigInt &y) const {
    BigInt a = x;
    BigInt b = y;
    BigInt p = poly;
    p.makePoly();
    a.makePoly();
    b.makePoly();
    return (a + b) % p;
}

BigInt EllipticCurve::gf_mul(const BigInt &x, const BigInt &y) const {
    BigInt a = x;
    BigInt b = y;
    BigInt p = poly;
    p.makePoly();
    a.makePoly();
    b.makePoly();
    return (a * b) % p;
}

bool EllipticCurve::check_point(const EllipticPoint &p) const {
    BigInt left = gf_add(gf_mul(p.y, p.y), gf_mul(p.x, p.y));
    BigInt x2 = gf_mul(p.x, p.x);
    BigInt x3 = gf_mul(p.x, x2);
    BigInt ax2 = gf_mul(A, x2);
    BigInt right = gf_add(x3, gf_add(ax2, B));
    return left == right;
}

EllipticPoint EllipticCurve::add_points(const EllipticPoint &p, const EllipticPoint &q) const {
    if (p.is_zero) {
        return q;
    }
    if (q.is_zero) {
        return p;
    }
    BigInt x3;
    BigInt y3;
    BigInt k;
    if (p.x == q.x) {
        k = gf_mul(gf_add(gf_mul(p.x, p.x), p.y), gf_inv(p.x));
    } else {
        k = gf_mul(gf_add(p.y, q.y), gf_inv(gf_add(p.x, q.x)));
    }
    x3 = gf_add(gf_mul(k, k), gf_add(k, gf_add(A, gf_add(p.x, q.x))));
    y3 = gf_add(gf_mul(gf_add(p.x, x3), k), gf_add(x3, p.y));
    return EllipticPoint(x3, y3);
}

BigInt EllipticCurve::gf_inv(const BigInt &a) const {
    //return gf_pow(x, (BigInt(1) << (m - 2)));
    BigInt x, y;
    BigInt AA = a;
    BigInt BB = poly;
    AA.makePoly();
    BB.makePoly();

    BigInt gcd = gcd_extended(AA, BB, x, y);
    BigInt r = AA * x + BB * y;
    return x;
}

BigInt EllipticCurve::gf_pow(const BigInt &x, BigInt y) const {
    BigInt tmp = x;
    BigInt res = 1;
    while (!y.isZero()) {
        if (y.isOdd()) {
            res = gf_mul(res, tmp);
        }
        y = y >> 1;
        tmp = gf_mul(tmp, tmp);
    }
    return res;
}

EllipticPoint EllipticCurve::mul_point(const EllipticPoint &a, BigInt k) const {
    EllipticPoint res;
    EllipticPoint tmp = a;
    while (!k.isZero()) {
        if (k.isOdd()) {
            res = add_points(res, tmp);
        }
        tmp = add_points(tmp, tmp);
        k = k >> 1;
    }
    return res;
}

EllipticPoint EllipticCurve::double_point(const EllipticPoint &a) const {
    return add_points(a, a);
}

BigInt EllipticCurve::get_n() const {
    return n;
}


BigInt gcd_extended(BigInt a, BigInt b, BigInt &x, BigInt &y) {
    a.makePoly();
    b.makePoly();
    x.makePoly();
    y.makePoly();
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    BigInt x1, y1;
    x1.makePoly();
    y1.makePoly();
    BigInt gcd = gcd_extended(b % a, a, x1, y1);
    x1.makePoly();
    y1.makePoly();
    x = y1 + (b / a) * x1;
    y = x1;
    return gcd;
}


namespace EllipticCurveSignature {
    std::pair<PrivateKey, PublicKey> GenerateKeys(const EllipticCurve &curve) {
        BigInt x = BigInt::rand(1, BigInt(1) << 32);
        return {PrivateKey{x},
                PublicKey{curve.mul_point(curve.get_g(), x)}};
    }

    Signature Sign(const BigInt &message, const PrivateKey &key, const EllipticCurve &curve) {
        BigInt q = curve.get_n();
        BigInt k = BigInt::rand(1, curve.get_n());
        BigInt r = curve.mul_point(curve.get_g(), k).x % q;
        BigInt ik = BigInt::inverseModulo(k, q);
        BigInt s = (ik * (message + key.x * r)) % q;
        return {r, s};
    }

    bool CheckSign(const BigInt &message, const Signature & sign, const PublicKey &key, const EllipticCurve &curve) {
        BigInt q = curve.get_n();
        EllipticPoint P = curve.get_g();
        BigInt u1 = BigInt::inverseModulo(sign.s, q) * message % q;
        BigInt u2 = BigInt::inverseModulo(sign.s, q) * sign.r % q;
        BigInt v = curve.add_points(curve.mul_point(P, u1), curve.mul_point(key.Q, u2)).x;
        return v == sign.r;
    }
}

EllipticPoint::EllipticPoint(const BigInt &a, const BigInt &b):x(a), y(b), is_zero(false) {
    x.makePoly(false);
    y.makePoly(false);
}

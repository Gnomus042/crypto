//
// Created by anast on 11/14/2020.
//

#include <cassert>
#include "RSA.h"
#include "../Lab3/Kupyna.h"
#include "../Lab3/SHA256.h"

const int RSA_SIZE = 1024;

pair<Key, Key> RSA::keygen(int prime_size) {
    std::cout << "Generating primes" << std::endl;
    BigInt p = get_prime(prime_size);
    std::cout << "Prime p generated." << std::endl;
    BigInt q = get_prime(prime_size);
    std::cout << "Prime q generated." << std::endl;
    BigInt N = p * q;
    BigInt phi = (p - 1) * (q - 1);
    BigInt e = BigInt::rand(BigInt::pow(2, RSA_SIZE - 1), BigInt::pow(2, RSA_SIZE));
    while (BigInt::gcd(e, phi) != 1)
        e = BigInt::rand(BigInt::pow(2, RSA_SIZE - 1), BigInt::pow(2, RSA_SIZE));
    BigInt d = BigInt::inverseModulo(e, phi);
    Key pk = {N, e};
    Key sk = {N, d};
    return {pk, sk};
}

BigInt RSA::get_prime(int prime_size) {
    BigInt x = BigInt::rand(BigInt::pow(2, prime_size - 1), BigInt::pow(2, prime_size));
    if (x % 2 == 0) x += 1;
    while (!MillerRabin::test(x, 10)) {
        x += 2;
    }
    return x;
}

vector<uint8_t> RSA::encrypt(const vector<uint8_t> &m, const Key &pk) {
    vector<uint8_t> encrypted = BigInt::mpow(m, pk.val, pk.mod).tou8();
    while(encrypted.size() < m.size()) encrypted.push_back(0);
    return encrypted;
}

vector<uint8_t> RSA::decrypt(const vector<uint8_t> &c, const Key &sk) {
    return BigInt::mpow(c, sk.val, sk.mod).tou8();
}

vector<uint8_t> hashG(const vector<uint8_t> m, int size) {
    vector<uint8_t> res;
    vector<uint8_t> hashm = SHA256::hash(m);
    res.insert(res.end(), hashm.begin(), hashm.end());
    res.insert(res.end(), hashm.begin(), hashm.end());
    res.insert(res.end(), hashm.begin(), hashm.end());
    res.insert(res.end(), hashm.begin(), hashm.end());
    res.resize(size);
    return res;
}

vector<uint8_t> RSA::decrypt_oaep(const vector<uint8_t> &m, const Key &sk, int block_size) {
    int k0 = 32;
    int k1 = 32;
    Kupyna kupyna(256);
    vector<uint8_t> mes = m;
    vector<uint8_t> res;
    for (int k = 0; k < mes.size() / block_size / 2; k++) {
        vector<uint8_t> block(mes.begin() + k * block_size * 2, mes.begin() + (k + 1) * block_size * 2);
        block = RSA::decrypt(block, sk);
        while (block.size() < block_size) block.push_back(0);
        vector<uint8_t> Y(block.end()-k0, block.end());
        vector<uint8_t> X(block.begin(), block.end()-k0);
        vector<uint8_t> HX = kupyna.hash(X);
        vector<uint8_t> r = Y;
        assert(r.size() == HX.size());
        for (int i = 0; i < r.size(); i++) r[i] = r[i] ^ HX[i];
        vector<uint8_t> Gr = hashG(r, block_size-k0);
        assert(Gr.size() == X.size());
        for (int i = 0; i < r.size(); i++) Gr[i] = Gr[i] ^ X[i];
        res.insert(res.end(), Gr.begin(), Gr.end()-k1);
    }
    return res;
}

vector<uint8_t> RSA::encrypt_oaep(const vector<uint8_t> &m, const Key &pk, int block_size) {
    int k0 = 32;
    int k1 = 32;
    vector<uint8_t> mes = m;
    while (mes.size() % (block_size-k0-k1) != 0) {
        mes.push_back(0);
    }
    vector<uint8_t> c;
    Kupyna kupyna(256);
    for (int k = 0; k < mes.size() / (block_size-k0-k1); k++) {
        vector<uint8_t> r(k0);
        vector<uint8_t> block(mes.begin() + k * (block_size-k0-k1), mes.begin() + (k + 1) * (block_size-k0-k1));
        for (int i = 0; i < k1; i++) block.push_back(0);
        for (int i = 0; i < k0; i++) r[i] = rand() % 256;
        vector<uint8_t> Gr = hashG(r, block_size-k0);
        vector<uint8_t> X = block;
        assert(X.size() == Gr.size());
        for (int i = 0; i < Gr.size(); i++) X[i] = X[i] ^ Gr[i];
        vector<uint8_t> HX = kupyna.hash(X);
        vector<uint8_t> Y = r;
        assert(Y.size() == HX.size());
        for (int i = 0; i < Y.size(); i++) Y[i] = Y[i] ^ HX[i];
        X.insert(X.end(), Y.begin(), Y.end());
        vector<uint8_t> encrypted = RSA::encrypt(X, pk);
        c.insert(c.cend(), encrypted.begin(), encrypted.end());
    }
    return c;
}

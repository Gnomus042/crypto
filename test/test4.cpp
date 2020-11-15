//
// Created by anast on 11/14/2020.
//

#include <gtest/gtest.h>
#include "../Lab4/MillerRabin.h"
#include "../Lab4/RSA.h"

TEST(MillerRabin, MillerRabin) {
    EXPECT_EQ(MillerRabin::test(11, 10), true);
    EXPECT_EQ(MillerRabin::test(45, 10), false);
    EXPECT_EQ(MillerRabin::test(9973, 10), true);
    EXPECT_EQ(MillerRabin::test(9957, 10), false);
}

TEST(RSA, encrypt_decrypt) {
    string data_str = "Absolutely random sentence";
    vector<uint8_t> data(begin(data_str), end(data_str));

    RSA rsa;
    auto keys = rsa.keygen(128);

    string res = "";
    for (uint8_t q: rsa.decrypt(rsa.encrypt(data, keys.first), keys.second)) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST(RSA, oaep_encrypt_decrypt) {
    string data_str = "Absolutely random sentence";
    vector<uint8_t> data(begin(data_str), end(data_str));

    RSA rsa;
    auto keys = rsa.keygen(1024);

    string res = "";
    vector<uint8_t> encrypted = rsa.encrypt_oaep(data, keys.first, 128);
    for (uint8_t q: rsa.decrypt_oaep(encrypted, keys.second, 128)) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}


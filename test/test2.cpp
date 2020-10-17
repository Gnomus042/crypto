//
// Created by anast on 10/11/2020.
//

#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "../Lab1/AES.h"
#include "../Lab2/RC4.h"
#include "../Lab2/Salsa20.h"


TEST (RC4, encrypt_decrypt)
{
    string key_str = "12345678901234561234567890123456";
    string data_str = "Absolutely random sentence";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    RC4 rc4 = RC4(key);
    string res = "";
    for (uint8_t q: rc4.decrypt(rc4.encrypt(data))) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (Salsa20, encrypt_decrypt)
{
    string key_str = "12345678901234561234567890123456";
    string data_str = "Absolutely random sentence";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    Salsa20 salsa20 = Salsa20(key);
    string res = "";
    for (uint8_t q: salsa20.crypt(salsa20.crypt(data))) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (AES, 128_key_ecb)
{
    string key_str = "1234567890123456";
    string data_str = "Absolutely random sentence";
    string init_vector_str = "This is an initt";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> init_vector(begin(init_vector_str), end(init_vector_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    AES aes(key, init_vector);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data, AES::Mode::ECB), AES::Mode::ECB)) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (AES, 128_key_cbc)
{
    string key_str = "1234567890123456";
    string data_str = "Absolutely random sentence";
    string init_vector_str = "This is an initt";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> init_vector(begin(init_vector_str), end(init_vector_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    AES aes(key, init_vector);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data, AES::Mode::CBC), AES::Mode::CBC)) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (AES, 128_key_ofb)
{
    string key_str = "1234567890123456";
    string data_str = "Absolutely random sentence";
    string init_vector_str = "This is an initt";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> init_vector(begin(init_vector_str), end(init_vector_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    AES aes(key, init_vector);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data, AES::Mode::OFB), AES::Mode::OFB)) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (AES, 128_key_ctr)
{
    string key_str = "1234567890123456";
    string data_str = "Absolutely random sentence";
    string init_vector_str = "This is an initt";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> init_vector(begin(init_vector_str), end(init_vector_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    AES aes(key, init_vector);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data, AES::Mode::CTR), AES::Mode::CTR)) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (AES, 128_key_cfb)
{
    string key_str = "1234567890123456";
    string data_str = "Absolutely random sentence";
    string init_vector_str = "This is an initt";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> init_vector(begin(init_vector_str), end(init_vector_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    AES aes(key, init_vector);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data, AES::Mode::CFB), AES::Mode::CFB)) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}



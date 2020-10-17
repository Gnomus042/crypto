//
// Created by anast on 10/4/2020.
//

#include <gtest/gtest.h>
#include "../Lab1/AES.h"
#include "../Lab1/Kalyna.h"
#include <iostream>

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST (AES, 128_key)
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

TEST (AES, 256_key)
{
    string key_str = "12345678901234561234567890123456";
    string data_str = "Absolutely random sentence";
    string init_vector_str = "This is an init, this is an init";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));
    vector<uint8_t> init_vector(begin(init_vector_str), end(init_vector_str));

    AES aes(key, init_vector);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data, AES::Mode::ECB), AES::Mode::ECB)) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (AES, 192_key)
{
    string key_str = "123456789012345612345678";
    string data_str = "Absolutely random sentence";
    string init_vector_str = "This is an init, this is";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));
    vector<uint8_t> init_vector(begin(init_vector_str), end(init_vector_str));

    AES aes(key, init_vector);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data, AES::Mode::ECB), AES::Mode::ECB)) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (Kalyna, 128_key_128_data)
{
    string key_str = "1234567890123456";
    string data_str = "Absolutely random sentence";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    Kalyna kalyna(key, 128, 128);
    string res = "";
    for (uint8_t q: kalyna.decrypt(kalyna.encrypt(data))) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (Kalyna, 256_key_128_data)
{
    string key_str = "12345678901234561234567890123456";
    string data_str = "Absolutely random sentence";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    Kalyna kalyna(key, 256, 128);
    string res = "";
    for (uint8_t q: kalyna.decrypt(kalyna.encrypt(data))) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (Kalyna, 256_key_256_data)
{
    string key_str = "12345678901234561234567890123456";
    string data_str = "Absolutely random sentence. Just very-very random.";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    Kalyna kalyna(key, 256, 256);
    string res = "";
    for (uint8_t q: kalyna.decrypt(kalyna.encrypt(data))) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (Kalyna, 512_key_256_data)
{
    string key_str = "1234567890123456123456789012345612345678901234561234567890123456";
    string data_str = "Absolutely random sentence. Just very-very random.";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    Kalyna kalyna(key, 512, 256);
    string res = "";
    for (uint8_t q: kalyna.decrypt(kalyna.encrypt(data))) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (Kalyna, 512_key_512_data)
{
    string key_str = "1234567890123456123456789012345612345678901234561234567890123456";
    string data_str = "Absolutely random sentence. Just very-very random. Leave it here.";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    Kalyna kalyna(key, 512, 512);
    string res = "";
    for (uint8_t q: kalyna.decrypt(kalyna.encrypt(data))) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}


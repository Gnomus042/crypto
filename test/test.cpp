//
// Created by anast on 10/4/2020.
//

#include <gtest/gtest.h>
#include "../AES.h"

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST (AES, 128_key)
{
    string key_str = "1234567890123456";
    string data_str = "Absolutely random sentence";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    AES aes(key);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data))) {
           res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (AES, 256_key)
{
    string key_str = "12345678901234561234567890123456";
    string data_str = "Absolutely random sentence";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    AES aes(key);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data))) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}

TEST (AES, 192_key)
{
    string key_str = "123456789012345612345678";
    string data_str = "Absolutely random sentence";

    vector<uint8_t> key(begin(key_str), end(key_str));
    vector<uint8_t> data(begin(data_str), end(data_str));

    AES aes(key);
    string res = "";
    for (uint8_t q: aes.decrypt(aes.encrypt(data))) {
        res += (char)q;
    }
    res = res.substr(0, data.size());
    EXPECT_EQ(res, data_str);
}
//
// Created by anast on 10/31/2020.
//

#include <gtest/gtest.h>
#include "../Lab3/SHA256.h"
#include "../Lab3/Kupyna.h"

uint8_t to_byte(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return c - 'A' + 10;
}

uint8_t to_byte(char a, char b) {
    return (to_byte(a) << 4) + to_byte(b);
}

vector<uint8_t> hex_to_bytes(const string &str) {
    assert(str.size() % 2 == 0);
    vector<uint8_t> data(str.size() / 2);
    for (int i = 0; i * 2 + 1 < str.size(); i++) {
        data[i] = to_byte(str[i * 2], str[i * 2 + 1]);
    }
    return data;
}

TEST (SHA256, hash) {
    string data_str = "hello";
    vector<uint8_t> data(begin(data_str), end(data_str));
    vector<uint8_t> actual = SHA256::hash(data);
    vector<uint8_t> expected = {0x2c, 0xf2, 0x4d, 0xba, 0x5f, 0xb0, 0xa3, 0x0e, 0x26,
                                0xe8, 0x3b, 0x2a, 0xc5, 0xb9, 0xe2, 0x9e, 0x1b, 0x16,
                                0x1e, 0x5c, 0x1f, 0xa7, 0x42, 0x5e, 0x73, 0x04, 0x33,
                                0x62, 0x93, 0x8b, 0x98, 0x24};
    EXPECT_EQ(actual, expected);
}


TEST(Kupyna, hash_256_long) {
    string data_str = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F"
                      "202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F"
                      "404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F"
                      "606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F"
                      "808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9F"
                      "A0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
                      "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
                      "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";
    vector<uint8_t> data = hex_to_bytes(data_str);
    string expected_str = "D305A32B963D149DC765F68594505D4077024F836C1BF03806E1624CE176C08F";
    vector<uint8_t> expected = hex_to_bytes(expected_str);
    Kupyna kupyna = Kupyna(256);
    EXPECT_EQ(kupyna.hash(data), expected);
}

TEST(Kupyna, hash_256) {
    string data_str = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F"
                      "202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F";
    vector<uint8_t> data = hex_to_bytes(data_str);
    string expected_str = "08F4EE6F1BE6903B324C4E27990CB24EF69DD58DBE84813EE0A52F6631239875";
    vector<uint8_t> expected = hex_to_bytes(expected_str);
    Kupyna kupyna = Kupyna(256);
    EXPECT_EQ(kupyna.hash(data), expected);
}

TEST(Kupyna, hash_512) {
    string data_str = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F"
                      "202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F";
    vector<uint8_t> data = hex_to_bytes(data_str);
    string expected_str = "3813E2109118CDFB5A6D5E72F7208DCCC80A2DFB3AFDFB02F46992B5EDBE536B"
                          "3560DD1D7E29C6F53978AF58B444E37BA685C0DD910533BA5D78EFFFC13DE62A";
    vector<uint8_t> expected = hex_to_bytes(expected_str);
    Kupyna kupyna = Kupyna(512);
    EXPECT_EQ(kupyna.hash(data), expected);
}

TEST(Kupyna, hash_384) {
    string data_str = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F"
                      "202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F"
                      "404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E";
    vector<uint8_t> data = hex_to_bytes(data_str);
    string expected_str = "D9021692D84E5175735654846BA751E6D0ED0FAC36DFBC0841287DCB0B5584C7"
                          "5016C3DECC2A6E47C50B2F3811E351B8";
    vector<uint8_t> expected = hex_to_bytes(expected_str);
    Kupyna kupyna = Kupyna(384);
    EXPECT_EQ(kupyna.hash(data), expected);
}

bool check_pow(int n, const vector<uint8_t>&  res) {
    assert(n < res.size());
    for (int i = 0; i < n; i++) {
        if (res[i]!=0) return false;
    }
    return true;
}

TEST (SHA256, pow) {
    vector<uint8_t> random_seq = random_data(20);
    vector<uint8_t> hash = SHA256::hash(random_seq);
    while(!check_pow(1, hash)) {
        random_seq = random_data(20);
        hash = SHA256::hash(random_seq);
    }
    EXPECT_EQ(0, 0);
}


TEST (Kupyna256, pow) {
    vector<uint8_t> random_seq = random_data(20);
    Kupyna kupyna = Kupyna(256);
    vector<uint8_t> hash = kupyna.hash(random_seq);
    while(!check_pow(1, hash)) {
        random_seq = random_data(20);
        hash = SHA256::hash(random_seq);
    }
    EXPECT_EQ(0, 0);
}

TEST (Kupyna384, pow) {
    vector<uint8_t> random_seq = random_data(20);
    Kupyna kupyna = Kupyna(384);
    vector<uint8_t> hash = kupyna.hash(random_seq);
    while(!check_pow(1, hash)) {
        random_seq = random_data(20);
        hash = SHA256::hash(random_seq);
    }
    EXPECT_EQ(0, 0);
}

TEST (Kupyna512, pow) {
    vector<uint8_t> random_seq = random_data(20);
    Kupyna kupyna = Kupyna(512);
    vector<uint8_t> hash = kupyna.hash(random_seq);
    while(!check_pow(1, hash)) {
        random_seq = random_data(20);
        hash = SHA256::hash(random_seq);
    }
    EXPECT_EQ(0, 0);
}
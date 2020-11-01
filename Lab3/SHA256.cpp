//
// Created by anast on 10/31/2020.
//

#include "SHA256.h"
#include <bitset>

vector<uint32_t> SHA256::H {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
                            0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
vector<uint32_t> SHA256::K {0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
                            0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
                            0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
                            0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
                            0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
                            0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
                            0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
                            0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

vector<uint8_t> SHA256::hash(const vector<uint8_t>& message) {
    vector<uint32_t> h = H;
    vector<uint8_t> m = message;
    if (m.size() == 0) {
        throw "Message is empty";
    }
    m.push_back(128);
    while (m.size()*8 % 512 != 448) {
        m.push_back(0);
    }
    vector<uint8_t> sbytes = u64to8(message.size()*8);
    reverse(sbytes.begin(), sbytes.end());
    m.insert(m.end(), sbytes.begin(), sbytes.end());


    for (int i = 0; i < m.size()/64; i++) {
        vector<uint32_t> words(64);
        for (int j = 0; j < 64; j++) {
            if (j < 16) {
                words[j] = u8to32(m.data() + i * 64 + j * 4);
            } else {
                uint32_t s0 = u32rotr(words[j-15], 7) ^ u32rotr(words[j-15], 18) ^ (words[j-15] >> 3);
                uint32_t s1 = u32rotr(words[j-2], 17) ^ u32rotr(words[j-2], 19) ^ (words[j-2] >> 10);
                words[j] = words[j-16] + s0 + words[j-7] + s1;
            }
        }

        vector<uint32_t> temp = h;
        for (int j = 0; j < 64; j++) {
            uint32_t sigm0 = u32rotr(temp[0], 2) ^ u32rotr(temp[0], 13) ^ u32rotr(temp[0], 22);
            uint32_t ma = (temp[0] & temp[1]) ^ (temp[0] & temp[2]) ^ (temp[1] & temp[2]);
            uint32_t t1 = sigm0 + ma;
            uint32_t sigm1 = u32rotr(temp[4], 6) ^ u32rotr(temp[4], 11) ^ u32rotr(temp[4], 25);
            uint32_t ch = (temp[4] & temp[5]) ^ ((~temp[4]) & temp[6]);
            uint32_t t2 = temp[7] + sigm1 + ch + K[j] + words[j];

            for (int k = 7; k > 0; k--) temp[k] = temp[k-1];
            temp[0] = t1 + t2;
            temp[4] = temp[4] + t2;
        }

        for (int j = 0; j < 8; j++) {
            h[j] += temp[j];
        }
    }
    vector<uint8_t> res(32);
    for (int i = 0; i < h.size(); i++) {
        vector<uint8_t> u8temp = u32to8(h[i]);
        for (int j = 0; j < u8temp.size(); j++)
            res[i*u8temp.size()+j] = u8temp[j];
    }
    return res;
}
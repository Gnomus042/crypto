//
// Created by anast on 10/10/2020.
//

#include "RC4.h"

RC4::RC4(vector<uint8_t> key) {
    key_block = Block(key.data(), 1, key.size());
}

void RC4::make_key_schedule(Block &key) {
    Block s = Block(1, 256);
    for (int i = 0; i < 256; i++) {
        s.data[0][i] = {(uint8_t) i};
    }
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + s.data[0][i].data + key.data[0][(i % key.y)].data) % 256;
        Byte temp = s.data[0][j];
        s.data[0][j] = s.data[0][i];
        s.data[0][i] = temp;
    }
    key_schedule = s;
}

void RC4::pseudo_random_generation(Block& data) {
    make_key_schedule(key_block);
    int i = 0;
    int j = 0;
    int q = 0;
    while (q < data.y) {
        i = (i + 1) % 256;
        j = (j + key_schedule.data[0][i].data) % 256;
        Byte temp = key_schedule.data[0][i];
        key_schedule.data[0][i] = key_schedule.data[0][j];
        key_schedule.data[0][j] = temp;
        Byte k = key_schedule.data[0][static_cast<uint8_t>(key_schedule.data[0][i].data + key_schedule.data[0][j].data)];
        data.data[0][q] = data.data[0][q] + k;
        q++;
    }
}

vector<uint8_t> RC4::encrypt(const vector<uint8_t>& data) {
    Block data_block = Block(data.data(), 1, data.size());
    pseudo_random_generation(data_block);
    return data_block.get_data();
}

vector<uint8_t> RC4::decrypt(const vector<uint8_t> &data) {
    Block data_block = Block(data.data(), 1, data.size());
    pseudo_random_generation(data_block);
    return data_block.get_data();
}

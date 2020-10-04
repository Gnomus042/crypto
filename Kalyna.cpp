//
// Created by anast on 10/4/2020.
//

#include "Kalyna.h"
#include "kalyna_tables.h"

Kalyna::Kalyna(const vector<uint8_t> &key, int key_size, int data_size) {
    this->key_length = key_size;
    this->data_length = data_size;
    this->sboxes = {sbox0, sbox1, sbox2, sbox3};
    this->inv_sboxes = {isbox0, isbox1, isbox2, isbox3};
    Block key_block = Block(key.data(), 8, 2);
    this->make_key_schedule(key_block, 11);
}

vector<uint8_t> Kalyna::encrypt(vector<uint8_t> data) {
    vector<Block> blocks;
    while (data.size() % 16 != 0) data.push_back(0);
    for (int k = 0; k < data.size() / 16; k++) {
        blocks.push_back(Block(data.data() + k * 16, 8, 2));
    }
    vector<uint8_t> res;
    for (const Block &block: blocks) {
        vector<uint8_t> encrypted = this->encrypt_block(block).get_data();
        res.insert(end(res), begin(encrypted), end(encrypted));
    }
    return res;
}

vector<uint8_t> Kalyna::decrypt(vector<uint8_t> data) {
    vector<Block> blocks;
    while (data.size() % 16 != 0) data.push_back(0);
    for (int k = 0; k < data.size() / 16; k++) {
        blocks.push_back(Block(data.data() + k * 16, 8, 2));
    }
    vector<uint8_t> res;
    for (const Block &block: blocks) {
        vector<uint8_t> decrypted = this->decrypt_block(block).get_data();
        res.insert(end(res), begin(decrypted), end(decrypted));
    }
    return res;
}

Block Kalyna::encrypt_block(const Block &data) {
    vector<Byte> mult = {{1}, {1}, {5}, {1}, {8}, {6}, {7}, {4}};
    Block encrypted(data);
    add_round_key(encrypted, this->key_schedule[0]);
    for (int i = 1; i < 10; i++) {
        sub_bytes(encrypted, this->sboxes);
        this->shift_rows(encrypted);
        mix_columns(encrypted, mult);
        xor_round_key(encrypted, this->key_schedule[i]);
    }
    sub_bytes(encrypted, this->sboxes);
    this->shift_rows(encrypted);
    mix_columns(encrypted, mult);
    add_round_key(encrypted, this->key_schedule[10]);
    return encrypted;
}

Block Kalyna::decrypt_block(const Block &data) {
    vector<Byte> mult = {{0xAD}, {0x95}, {0x76}, {0xA8}, {0x2F}, {0x49}, {0xD7}, {0xCA}};
    Block decrypted(data);
    sub_round_key(decrypted, this->key_schedule[10]);
    mix_columns(decrypted, mult);
    this->inv_shift_rows(decrypted);
    sub_bytes(decrypted, this->inv_sboxes);
    for (int i = 9; i >= 1; i--) {
        xor_round_key(decrypted, this->key_schedule[i]);
        mix_columns(decrypted, mult);
        this->inv_shift_rows(decrypted);
        sub_bytes(decrypted, this->inv_sboxes);
    }
    sub_round_key(decrypted, this->key_schedule[0]);
    return decrypted;
}

Block Kalyna::make_intermediate_key(const Block &key) {
    Block start_value = Block(8, 2);
    start_value.data[0][0] = {static_cast<uint8_t>((this->key_length + this->data_length + 64) / 64)};
    vector<Byte> mult = {{1}, {1}, {5}, {1}, {8}, {6}, {7}, {4}};

    add_round_key(start_value, key);
    sub_bytes(start_value, sboxes);
    this->shift_rows(start_value);
    mix_columns(start_value, mult);
    xor_round_key(start_value, key);
    sub_bytes(start_value, sboxes);
    this->shift_rows(start_value);
    mix_columns(start_value, mult);
    add_round_key(start_value, key);
    sub_bytes(start_value, sboxes);
    this->shift_rows(start_value);
    mix_columns(start_value, mult);
    return start_value;
}

void Kalyna::make_key_schedule(Block &key, int blocks_count) {
    Block intermediate_key = this->make_intermediate_key(key);
    Block tempKey(key);
    Block tmv(8, 2);
    for (int i = 0; i < tmv.x; i++) {
        for (int j = 0; j < tmv.y; j++) {
            tmv.data[i][j].data = (i+1)%2;
        }
    }
    vector<Byte> mult = {{1}, {1}, {5}, {1}, {8}, {6}, {7}, {4}};
    for (int i = 0; i < blocks_count; i++) {
        if (i%2 == 0) {
            Block round_key = even_round_key(tempKey, intermediate_key, i, tmv);
            round_key.print();
            this->key_schedule.push_back(round_key);
        } else {
            Block round_key(this->key_schedule[i-1]);
            round_key.left_rotate(key_length/4 + 24);
            round_key.print();
            this->key_schedule.push_back(round_key);
        }
    }
}

Block Kalyna::even_round_key(Block& key, Block& ikey, int iter, Block& tmv) {
    vector<Byte> mult = {{1}, {1}, {5}, {1}, {8}, {6}, {7}, {4}};

    Block kt_round(ikey);
    add_round_key(kt_round, tmv);

    Block round_key(kt_round);
    add_round_key(round_key, key);
    sub_bytes(round_key, this->sboxes);
    this->shift_rows(round_key);
    mix_columns(round_key, mult);
    xor_round_key(round_key, kt_round);
    sub_bytes(round_key, this->sboxes);
    this->shift_rows(round_key);
    mix_columns(round_key, mult);
    add_round_key(round_key, kt_round);

    tmv.left_shift(iter/2);
    key.right_rotate(32*iter);

    return round_key;
}

void Kalyna::shift_rows(Block &block) {
    for (int i = 0; i < block.x; i++) {
        int shift = (i * data_length/512)%block.y;
        if (shift != 0) {
            vector<Byte> temp;
            for (int j = 0; j < block.y; j++) {
                temp.push_back(block.data[i][j]);
            }
            rotate(temp.begin(), temp.begin()+shift, temp.end());
            for (int j = 0; j < block.y; j++) {
                block.data[i][j] = temp[j];
            }
        }
    }
}

void Kalyna::inv_shift_rows(Block &block) {
    for (int i = 0; i < block.x; i++) {
        int shift = (i * data_length/512)%block.y;
        if (shift != 0) {
            vector<Byte> temp;
            for (int j = 0; j < block.y; j++) {
                temp.push_back(block.data[i][j]);
            }
            rotate(temp.begin(), temp.begin()+(block.y - shift), temp.end());
            for (int j = 0; j < block.y; j++) {
                block.data[i][j] = temp[j];
            }
        }
    }
}


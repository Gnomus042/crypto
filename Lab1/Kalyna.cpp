//
// Created by anast on 10/4/2020.
//

#include "Kalyna.h"

Kalyna::Kalyna(const vector<uint8_t> &key, int key_size, int block_size) {
    this->key_size = key_size;
    this->block_size = block_size;
    this->columns = block_size == 128 ? 2 : block_size == 256 ? 4:8;
    if (block_size == 128) {
        this->rounds = key_size == 128? 11 : 15;
    } else if (block_size == 256) {
        this->rounds = key_size == 256? 15 : 19;
    } else {
        this->rounds = 19;
    }
    this->mult = {{1}, {1}, {5}, {1}, {8}, {6}, {7}, {4}};
    this->inv_mult = {{0xAD}, {0x95}, {0x76}, {0xA8}, {0x2F}, {0x49}, {0xD7}, {0xCA}};
    this->sboxes = {sbox0, sbox1, sbox2, sbox3};
    this->inv_sboxes = {isbox0, isbox1, isbox2, isbox3};
    Block key_block = Block(key.data(), 8, this->columns);
    this->make_key_schedule(key_block, this->rounds);
}

vector<uint8_t> Kalyna::encrypt(vector<uint8_t> data) {
    vector<Block> blocks;
    while (data.size() % (8*this->columns) != 0) data.push_back(0);
    for (int k = 0; k < data.size() / (8*this->columns); k++) {
        blocks.push_back(Block(data.data() + k * (8*this->columns), 8,
                this->columns));
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
    while (data.size() % (8*this->columns) != 0) data.push_back(0);
    for (int k = 0; k < data.size() / (8*this->columns); k++) {
        blocks.push_back(Block(data.data() + k * (8*this->columns), 8, this->columns));
    }
    vector<uint8_t> res;
    for (const Block &block: blocks) {
        vector<uint8_t> decrypted = this->decrypt_block(block).get_data();
        res.insert(end(res), begin(decrypted), end(decrypted));
    }
    return res;
}

Block Kalyna::encrypt_block(const Block &data) {
    Block encrypted(data);
    add_round_key(encrypted, this->key_schedule[0]);
    for (int i = 1; i < this->rounds-1; i++) {
        sub_bytes(encrypted, this->sboxes);
        this->shift_rows(encrypted);
        mix_columns(encrypted, mult);
        xor_round_key(encrypted, this->key_schedule[i]);
    }
    sub_bytes(encrypted, this->sboxes);
    this->shift_rows(encrypted);
    mix_columns(encrypted, mult);
    add_round_key(encrypted, this->key_schedule[this->rounds-1]);
    return encrypted;
}

Block Kalyna::decrypt_block(const Block &data) {
    Block decrypted(data);
    sub_round_key(decrypted, this->key_schedule[this->rounds-1]);
    mix_columns(decrypted, inv_mult);
    this->inv_shift_rows(decrypted);
    sub_bytes(decrypted, this->inv_sboxes);
    for (int i = this->rounds-2; i >= 1; i--) {
        xor_round_key(decrypted, this->key_schedule[i]);
        mix_columns(decrypted, inv_mult);
        this->inv_shift_rows(decrypted);
        sub_bytes(decrypted, this->inv_sboxes);
    }
    sub_round_key(decrypted, this->key_schedule[0]);
    return decrypted;
}

Block Kalyna::make_intermediate_key(const Block &key) {
    Block start_value = Block(8, this->columns);
    start_value.data[0][0] = {static_cast<uint8_t>((this->key_size + this->block_size + 64) / 64)};

    Block k_alph, k_omeg;
    if (this->key_size == this->block_size) {
        k_alph.copyFrom(key);
        k_omeg.copyFrom(key);
    } else if (this->key_size == 2*this->block_size){
        vector<uint8_t> key_data = key.get_data();
        k_alph = Block(key_data.data(), 8, this->columns);
        k_omeg = Block(key_data.data() + 8*this->columns, 8, this->columns);
    }

    add_round_key(start_value, k_alph);
    sub_bytes(start_value, sboxes);
    this->shift_rows(start_value);
    mix_columns(start_value, mult);
    xor_round_key(start_value, k_omeg);
    sub_bytes(start_value, sboxes);
    this->shift_rows(start_value);
    mix_columns(start_value, mult);
    add_round_key(start_value, k_alph);
    sub_bytes(start_value, sboxes);
    this->shift_rows(start_value);
    mix_columns(start_value, mult);
    return start_value;
}

void Kalyna::make_key_schedule(Block &key, int blocks_count) {
    Block intermediate_key = this->make_intermediate_key(key);
    Block tempKey(key);
    Block tempTempKey(key);
    Block tmv(8, this->columns);
    for (int i = 0; i < tmv.x; i++) {
        for (int j = 0; j < tmv.y; j++) {
            tmv.data[i][j].data = (i+1)%2;
        }
    }
    for (int i = 0; i < blocks_count; i++) {
        if (i%2 == 0) {
            tmv.left_shift(i/2);
            if (block_size == key_size) {
                tempKey.right_rotate(32 * i);
                tempTempKey.copyFrom(tempKey);
            } else {
                if (i%4 == 0) {
                    tempKey.right_rotate(16*i);
                    tempTempKey = Block(tempKey.get_data().data(), 8, this->columns);
                } else {
                    tempKey.right_rotate(64*(i/4));
                    tempTempKey = Block(tempKey.get_data().data()+8*this->columns, 8, this->columns);
                }
            }
            Block round_key = even_round_key(tempTempKey, intermediate_key, i, tmv);
            this->key_schedule.push_back(round_key);
        } else {
            Block round_key(this->key_schedule[i-1]);
            round_key.left_rotate(key_size / 4 + 24);
            this->key_schedule.push_back(round_key);
        }
    }
}

Block Kalyna::even_round_key(Block& key, Block& ikey, int iter, Block& tmv) {
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

    return round_key;
}

void Kalyna::shift_rows(Block &block) {
    for (int i = 0; i < block.x; i++) {
        int shift = (i * block_size / 512) % block.y;
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
        int shift = (i * block_size / 512) % block.y;
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


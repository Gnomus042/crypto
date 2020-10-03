//
// Created by anast on 9/26/2020.
//

#include "AES.h"

vector<uint8_t> rcon_header = {1, 2, 4, 8, 16, 32, 64, 128, 27, 54};

AES::AES(const vector<uint8_t> &key) {
    this->generate_sbox();
    this->generate_inv_sbox();
    this->rcon = Block(4, rcon_header.size());
    for (int i = 0; i < rcon_header.size(); i++) {
        this->rcon.data[0][i] = {rcon_header[i]};
    }
    Block key_block = Block(key.data(), 4, 4);
    this->make_key_schedule(key_block, 11);
}

vector<uint8_t> AES::encrypt(vector<uint8_t> data) {
    vector<Block> blocks;
    while (data.size() % 16 != 0) data.push_back(0);
    for (int k = 0; k < data.size() / 16; k++) {
        blocks.push_back(Block(data.data() + k * 16, 4, 4));
    }
    vector<uint8_t> res;
    for (const Block &block: blocks) {
        vector<uint8_t> encrypted = this->encrypt_block(block).get_data();
        res.insert(end(res), begin(encrypted), end(encrypted));
    }
    return res;
}

vector<uint8_t> AES::decrypt(vector<uint8_t> data) {
    vector<Block> blocks;
    while (data.size() % 16 != 0) data.push_back(0);
    for (int k = 0; k < data.size() / 16; k++) {
        blocks.push_back(Block(data.data() + k * 16, 4, 4));
    }
    vector<uint8_t> res;
    for (const Block &block: blocks) {
        vector<uint8_t> decrypted = this->decrypt_block(block).get_data();
        res.insert(end(res), begin(decrypted), end(decrypted));
    }
    return res;
}

void AES::generate_sbox() {
    this->sbox = vector<Byte>(256);
    for (int i = 0; i < 256; i++) {
        uint8_t temp = 0xf8;
        uint8_t res = 0;
        uint8_t inverted = Byte::invert({(uint8_t) i}).data;

        for (int j = 0; j < 8; j++) {
            uint8_t sum = 0;
            for (int k = 0; k < 8; k++) {
                sum ^= ((inverted & temp) >> k) & 1;
            }
            res = (res << 1) | sum;
            temp = (temp >> 1) | ((temp & 1) << 7);
        }

        this->sbox[i] = {static_cast<uint8_t>(res ^ (uint8_t) 0x63)};
    }
}

void AES::generate_inv_sbox() {
    this->inv_sbox = vector<Byte>(256);
    for (int i = 0; i < 256; i++) {
        this->inv_sbox[(int) this->sbox[i].data] = {(uint8_t) i};
    }
}

void AES::make_key_schedule(const Block &key, int blocks_count) {
    this->key_schedule = vector<Block>{key};
    for (int i = 1; i < blocks_count; i++) {
        Block new_block = Block(key.x, key.y);
        for (int j = 0; j < key.y; j++) {
            if (j % key.x == 0) {
                vector<Byte> last_column;
                for (int k = 0; k < key.x; k++) {
                    last_column.push_back(this->key_schedule[i - 1].data[k][key.y - 1]);
                }
                rotate(begin(last_column), begin(last_column) + 1, end(last_column));
                for (int k = 0; k < key.x; k++) {
                    last_column[k] = this->sbox[last_column[k].data];
                    new_block.data[k][j] =
                            last_column[k] + this->rcon.data[k][(i * key.y + j) / key.y - 1] +
                            key_schedule[i - 1].data[k][j];
                }
            } else {
                for (int k = 0; k < key.y; k++) {
                    new_block.data[k][j] =
                            new_block.data[k][j-1] + key_schedule[i - 1].data[k][j];
                }
            }
        }
        this->key_schedule.push_back(new_block);
    }
}

Block AES::encrypt_block(const Block &data) {
    Block encrypted(data.x, data.y);
    encrypted.copyFrom(data);
    vector<Byte> mult = {{2},
                         {3},
                         {1},
                         {1}};
    add_round_key(encrypted, this->key_schedule[0]);
    for (int i = 1; i < 10; i++) {
        sub_bytes(encrypted, this->sbox);
        shift_rows(encrypted);
        mix_columns(encrypted, mult);
        add_round_key(encrypted, this->key_schedule[i]);
    }
    sub_bytes(encrypted, this->sbox);
    shift_rows(encrypted);
    add_round_key(encrypted, this->key_schedule[10]);
    return encrypted;
}

Block AES::decrypt_block(const Block &data) {
    Block decrypted(data.x, data.y);
    decrypted.copyFrom(data);
    vector<Byte> mult = {{14},
                         {11},
                         {13},
                         {9}};
    add_round_key(decrypted, this->key_schedule[10]);
    for (int i = 9; i >= 1; i--) {
        inv_shift_rows(decrypted);
        sub_bytes(decrypted, this->inv_sbox);
        add_round_key(decrypted, this->key_schedule[i]);
        mix_columns(decrypted, mult);
    }
    inv_shift_rows(decrypted);
    sub_bytes(decrypted, this->inv_sbox);
    add_round_key(decrypted, this->key_schedule[0]);
    return decrypted;
}








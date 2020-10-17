//
// Created by anast on 9/26/2020.
//

#include "AES.h"

vector<uint8_t> rcon_header = {1, 2, 4, 8, 16, 32, 64, 128, 27, 54};

AES::AES(const vector<uint8_t> &key, const vector<uint8_t> &in_vector) {
    this->in_vector = in_vector;
    if (key.size() * 8 != 128 && key.size() * 8 != 192 && key.size() * 8 != 256) {
        cout << "Invalid key size provided";
        throw 1;
    }
    this->blocks_count = key.size() * 8 == 128 ? 11 : key.size() * 8 == 192 ? 13 : 15;
    this->generate_sbox();
    this->generate_inv_sbox();
    this->rcon = Block(4, rcon_header.size());
    for (int i = 0; i < rcon_header.size(); i++) {
        this->rcon.data[0][i] = {rcon_header[i]};
    }
    Block key_block = Block(key.data(), 4, 4);
    this->make_key_schedule(key_block, blocks_count);
}

vector<uint8_t> AES::encrypt(vector<uint8_t> data, Mode mode) {
    while (data.size() % 16 != 0) data.push_back(0);
    if (mode == Mode::CBC) return encrypt_cbc(data, true);
    if (mode == Mode::CFB) return encrypt_cfb(data, true);
    if (mode == Mode::CTR) return encrypt_ctr(data, true);
    if (mode == Mode::ECB) return encrypt_ecb(data, true);
    return encrypt_ofb(data, true);
}

vector<uint8_t> AES::decrypt(vector<uint8_t> data, Mode mode) {
    while (data.size() % 16 != 0) data.push_back(0);
    if (mode == Mode::CBC) return encrypt_cbc(data, false);
    if (mode == Mode::CFB) return encrypt_cfb(data, false);
    if (mode == Mode::CTR) return encrypt_ctr(data, false);
    if (mode == Mode::ECB) return encrypt_ecb(data, false);
    return encrypt_ofb(data, false);
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
                            new_block.data[k][j - 1] + key_schedule[i - 1].data[k][j];
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
    xor_round_key(encrypted, this->key_schedule[0]);
    for (int i = 1; i < blocks_count - 1; i++) {
        sub_bytes(encrypted, {this->sbox});
        shift_rows(encrypted);
        mix_columns(encrypted, mult);
        xor_round_key(encrypted, this->key_schedule[i]);
    }
    sub_bytes(encrypted, {this->sbox});
    shift_rows(encrypted);
    xor_round_key(encrypted, this->key_schedule[blocks_count - 1]);
    return encrypted;
}

Block AES::decrypt_block(const Block &data) {
    Block decrypted(data.x, data.y);
    decrypted.copyFrom(data);
    vector<Byte> mult = {{14},
                         {11},
                         {13},
                         {9}};
    xor_round_key(decrypted, this->key_schedule[blocks_count - 1]);
    for (int i = blocks_count - 2; i >= 1; i--) {
        inv_shift_rows(decrypted);
        sub_bytes(decrypted, {this->inv_sbox});
        xor_round_key(decrypted, this->key_schedule[i]);
        mix_columns(decrypted, mult);
    }
    inv_shift_rows(decrypted);
    sub_bytes(decrypted, {this->inv_sbox});
    xor_round_key(decrypted, this->key_schedule[0]);
    return decrypted;
}

vector<uint8_t> AES::encrypt_ecb(const vector<uint8_t> &data, bool is_encryption) {
    vector<Block> blocks;
    for (int k = 0; k < data.size() / 16; k++) {
        blocks.push_back(Block(data.data() + k * 16, 4, 4));
    }
    vector<uint8_t> res;
    res.reserve(data.size());
    for (const Block &block: blocks) {
        vector<uint8_t> crypted;
        if (is_encryption) {
            crypted = this->encrypt_block(block).get_data();
        } else {
            crypted = this->decrypt_block(block).get_data();
        }
        res.insert(end(res), begin(crypted), end(crypted));
    }
    return res;
}

vector<uint8_t> AES::encrypt_cbc(const vector<uint8_t> &data, bool is_encryption) {
    Block last_block(this->in_vector.data(), 4, 4);
    vector<uint8_t> res;
    res.reserve(data.size());
    for (int k = 0; k < data.size() / 16; k++) {
        Block data_block(data.data() + k * 16, 4, 4);
        vector<uint8_t> crypted;
        if (is_encryption) {
            last_block = this->encrypt_block(data_block + last_block);
            crypted = last_block.get_data();
        } else {
            crypted = (this->decrypt_block(data_block) + last_block).get_data();
            last_block = data_block;
        }
        res.insert(end(res), begin(crypted), end(crypted));
    }
    return res;
}

vector<uint8_t> AES::encrypt_ofb(vector<uint8_t> data, bool is_encryption) {
    Block last_block(this->in_vector.data(), 4, 4);
    vector<Block> blocks;
    while (data.size() > blocks.size() * 16) {
        last_block = encrypt_block(last_block);
        blocks.push_back(last_block);
    }
    vector<uint8_t> res;
    int data_idx = 0;
    for (const Block &block: blocks) {
        for (uint8_t val: block.get_data()) {
            if (data_idx >= data.size()) return res;
            res.push_back(val ^ data[data_idx]);
            data_idx++;
        }
    }
    return res;
}


vector<uint8_t> AES::encrypt_ctr(vector<uint8_t> data, bool is_encryption) {
    int counter = 0;
    vector<uint8_t> res;
    while (counter * 16 < data.size()) {
        vector<uint8_t> u8 = u32to8(counter);
        vector<uint8_t> counter_data(16);
        copy(u8.begin(), u8.end(), counter_data.end() - u8.size());
        Block block(counter_data.data(), 4, 4);
        vector<uint8_t> crypted = encrypt_block(block).get_data();
        res.insert(res.end(), crypted.begin(), crypted.end());
        counter++;
    }
    for (int i = 0; i < data.size(); i++) {
        data[i] = data[i] ^ res[i];
    }
    return data;
}

vector<uint8_t> AES::encrypt_cfb(vector<uint8_t> data, bool is_encryption) {
    int s = 4;
    Block encryption_block(this->in_vector.data(), 4, 4);
    vector<uint8_t> res;
    for (int i = 0; i < data.size() / s; i++) {
        vector<uint8_t> crypted;
        vector<uint8_t> next_encryption;
        if (is_encryption) {
            crypted = encrypt_block(encryption_block).get_data();
            crypted.erase(crypted.begin() + s, crypted.end());
            for (int j = 0; j < crypted.size(); j++) {
                crypted[j] = crypted[j] ^ data[s*i+j];
            }
            next_encryption = encryption_block.get_data();
            next_encryption.erase(next_encryption.begin(), next_encryption.end() - s);
            next_encryption.insert(next_encryption.end(), crypted.begin(), crypted.end());
        } else {
            crypted = encrypt_block(encryption_block).get_data();
            crypted.erase(crypted.begin() + s, crypted.end());
            for (int j = 0; j < crypted.size(); j++) {
                crypted[j] = crypted[j] ^ data[s*i+j];
            }
            next_encryption = encryption_block.get_data();
            next_encryption.erase(next_encryption.begin(), next_encryption.end() - s);
            next_encryption.insert(next_encryption.end(), data.begin() + s * i, data.begin() + s * (i + 1));
        }
        res.insert(res.end(), crypted.begin(), crypted.end());
        encryption_block = Block(next_encryption.data(), 4, 4);
    }
    return res;
}

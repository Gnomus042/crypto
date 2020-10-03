//
// Created by anast on 10/3/2020.
//

#include "helper.h"

Byte Byte::operator+(const Byte &other) {
    return {static_cast<uint8_t>(this->data ^ other.data)};
}

Byte Byte::operator*(const Byte &other) {
    uint8_t res = 0;
    uint8_t b = this->data;
    uint8_t a = other.data;
    while (b) {
        if (b & 1) res ^= a;
        if (a & 0x80) a = (a << 1) ^ 0x011b;
        else a <<= 1;
        b >>= 1;
    }
    return {res};
}

Byte Byte::pow(Byte val, int power) {
    uint8_t res = 1;
    if (power == 0) return {res};
    else if (power % 2 == 0) return Byte::pow(val * val, power / 2);
    else return Byte::pow(val * val, power / 2) * val;
}

Byte Byte::invert(const Byte &val) {
    return Byte::pow(val, 254);
}

Block::Block() {}

Block::Block(const uint8_t* data, int x, int y) {
    this->x = x;
    this->y = y;
    for (int i = 0; i < x; i++) this->data.push_back(vector<Byte>(y));
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            this->data[j][i] = {data[i*y+j]};
        }
    }
}

Block::Block(int x, int y) {
    this->x = x;
    this->y = y;
    for (int i = 0; i < x; i++) this->data.push_back(vector<Byte>(y));
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            this->data[j][i] = {0};
        }
    }
}

void Block::copyFrom(const Block& other) {
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            this->data[j][i] = other.data[j][i];
        }
    }
}

vector<uint8_t> Block::get_data() {
    vector<uint8_t> res;
    for(int i = 0; i < y; i++) {
        for(int j = 0; j < x; j++) {
            res.push_back(this->data[j][i].data);
        }
    }
    return res;
}

void Block::print() const {
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            cout << hex << (int)this->data[i][j].data << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void sub_bytes(Block& block, const vector<Byte> &substitution_box) {
    for (int i = 0; i < block.y; i++) {
        for (int j = 0; j < block.x; j++) {
            block.data[j][i] = substitution_box[block.data[j][i].data];
        }
    }
}

void shift_rows(Block& block) {
    for(int i = 0; i < block.x; i++) {
        vector<Byte> temp;
        for (int j = 0; j < block.y; j++) {
            temp.push_back(block.data[i][j]);
        }
        rotate(begin(temp), begin(temp)+i, end(temp));
        for (int j = 0; j < block.y; j++) {
            block.data[i][j] = temp[j];
        }
    }
}

void inv_shift_rows(Block& block) {
    for(int i = 0; i < block.x; i++) {
        vector<Byte> temp;
        for (int j = 0; j < block.y; j++) {
            temp.push_back(block.data[i][j]);
        }
        rotate(begin(temp), begin(temp)+(block.x - i), end(temp));
        for (int j = 0; j < block.y; j++) {
            block.data[i][j] = temp[j];
        }
    }
}

void mix_columns(Block& block, const vector<Byte>& mult) {
    Block new_block = Block(block.x, block.y);
    for (int i = 0; i < block.x; i++) {
        vector<Byte> temp = mult;
        for (int j = 0; j < block.y ; j++) {
            for (int k = 0; k < block.y; k++) {
                new_block.data[j][i] = new_block.data[j][i] + temp[k]*block.data[k][i];
            }
            rotate(begin(temp), end(temp)-1, end(temp));
        }

    }
    block.copyFrom(new_block);
}

void add_round_key(Block& block, const Block& key) {
    for (int i = 0; i < block.x; i++) {
        for (int j = 0; j < block.y; j++) {
            block.data[i][j] = block.data[i][j] + key.data[i][j];
        }
    }
}
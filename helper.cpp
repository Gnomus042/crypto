//
// Created by anast on 10/3/2020.
//

#include "helper.h"

Byte Byte::operator+(const Byte &other) const {
    return {static_cast<uint8_t>(this->data ^ other.data)};
}

Byte Byte::operator*(const Byte &other) const {
    uint8_t res = 0;
    uint8_t b = this->data;
    uint8_t a = other.data;
    while (b) {
        if (b & 1) res ^= a;
        if (a & 0x80) a = (a << 1) ^ 0x11D; //0x011b
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

Byte Byte::left_rotate(const Byte &val, int d) {
    return {static_cast<uint8_t>((val.data << d) | (val.data >> 32 - d))};
}

Block::Block() {}

Block::Block(const uint8_t *data, int x, int y) {
    this->x = x;
    this->y = y;
    for (int i = 0; i < x; i++) this->data.push_back(vector<Byte>(y));
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            this->data[j][i] = {data[i * x + j]};
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

void Block::copyFrom(const Block &other) {
    this->x = other.x;
    this->y = other.y;
    this->data.clear();
    for (int i = 0; i < x; i++) this->data.push_back(vector<Byte>(y));
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            this->data[j][i] = other.data[j][i];
        }
    }
}

vector<uint8_t> Block::get_data() const {
    vector<uint8_t> res;
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            res.push_back(this->data[j][i].data);
        }
    }
    return res;
}

void Block::print() const {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            cout << hex << (int) this->data[i][j].data << " ";
        }
        cout << endl;
    }
    cout << endl;
}

Block::Block(const Block &other) {
    this->copyFrom(other);
}

void Block::left_shift(int val) {
    for (int i = 0; i < y; i++) {
        auto col = get_col(i);
        col <<= 1;
        set_col(i, col);
    }
}

void Block::right_shift(int val) {
    for (int i = 0; i < y; i++) {
        auto col = get_col(i);
        col >>= 1;
        set_col(i, col);
    }
}

void Block::left_rotate(int val) {
    val /= 8;
    vector<uint8_t> temp_data = get_data();
    rotate(begin(temp_data), begin(temp_data) + val, end(temp_data));
    Block temp(temp_data.data(), x, y);
    copyFrom(temp);
}

void Block::right_rotate(int val) {
    uint64_t temp = get_col(y - 1);
    for (int i = y - 1; i > 0; i--) {
        set_col(i, get_col(i - 1));
    }
    set_col(0, temp);
}

uint64_t Block::get_col(int number) const {
    return static_cast<uint64_t>(this->data[0][number].data) |
           static_cast<uint64_t>(this->data[1][number].data) << 8 |
           static_cast<uint64_t>(this->data[2][number].data) << 16 |
           static_cast<uint64_t>(this->data[3][number].data) << 24 |
           static_cast<uint64_t>(this->data[4][number].data) << 32 |
           static_cast<uint64_t>(this->data[5][number].data) << 40 |
           static_cast<uint64_t>(this->data[6][number].data) << 48 |
           static_cast<uint64_t>(this->data[7][number].data) << 56;
}

void Block::set_col(int number, uint64_t val) {
    for (int i = 0; i < 8; i++) {
        data[i][number].data = (val >> (i * 8)) & 0xff;
    }
}

Block Block::operator+(const Block &other) const {
    Block res(*this);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            res.data[i][j] = res.data[i][j] + other.data[i][j];
        }
    }
    return res;
}

void Block::rotate_row(int n, int d) {
    vector<Byte> temp;
    for (int j = 0; j < y; j++) temp.push_back(data[n][j]);
    rotate(begin(temp), begin(temp) + temp.size() - d, end(temp));
    for (int j = 0; j < y; j++) data[n][j] = temp[j];
}

void Block::print() {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            cout << hex << (int) data[i][j].data << " ";
        }
        cout << endl;
    }
    cout << endl;
}


void sub_bytes(Block &block, const vector<vector<Byte>> &substitution_boxes) {
    for (int i = 0; i < block.y; i++) {
        for (int j = 0; j < block.x; j++) {
            block.data[j][i] = substitution_boxes[j % substitution_boxes.size()][block.data[j][i].data];
        }
    }
}

void shift_rows(Block &block) {
    for (int i = 0; i < block.x; i++) {
        vector<Byte> temp;
        for (int j = 0; j < block.y; j++) {
            temp.push_back(block.data[i][j]);
        }
        rotate(begin(temp), begin(temp) + i, end(temp));
        for (int j = 0; j < block.y; j++) {
            block.data[i][j] = temp[j];
        }
    }
}

void inv_shift_rows(Block &block) {
    for (int i = 0; i < block.x; i++) {
        vector<Byte> temp;
        for (int j = 0; j < block.y; j++) {
            temp.push_back(block.data[i][j]);
        }
        rotate(begin(temp), begin(temp) + (block.x - i), end(temp));
        for (int j = 0; j < block.y; j++) {
            block.data[i][j] = temp[j];
        }
    }
}

void mix_columns(Block &block, const vector<Byte> &mult) {
    Block new_block = Block(block.x, block.y);
    for (int i = 0; i < block.y; i++) {
        vector<Byte> temp = mult;
        for (int j = 0; j < block.x; j++) {
            for (int k = 0; k < mult.size(); k++) {
                new_block.data[j][i] = new_block.data[j][i] + temp[k] * block.data[k][i];
            }
            rotate(begin(temp), end(temp) - 1, end(temp));
        }

    }
    block.copyFrom(new_block);
}

void xor_round_key(Block &block, const Block &key) {
    for (int i = 0; i < block.x; i++) {
        for (int j = 0; j < block.y; j++) {
            block.data[i][j] = block.data[i][j] + key.data[i][j];
        }
    }
}

void add_round_key(Block &block, const Block &key) {
    for (int i = 0; i < block.y; i++) {
        uint64_t col = block.get_col(i) + key.get_col(i);
        block.set_col(i, col);
    }
}

void sub_round_key(Block &block, const Block &key) {
    for (int i = 0; i < block.y; i++) {
        uint64_t col = block.get_col(i) - key.get_col(i);
        block.set_col(i, col);
    }
}

uint32_t u8to32(const uint8_t *begin) {
    return static_cast<uint32_t>(*begin) << 24 |
           static_cast<uint32_t>(*(begin + 1)) << 16 |
           static_cast<uint32_t>(*(begin + 2)) << 8 |
           static_cast<uint32_t>(*(begin + 3));
}

vector<uint8_t> u32to8(uint32_t data) {
    vector<uint8_t> res(4);
    res[0] = data >> 24;
    res[1] = data >> 16;
    res[2] = data >> 8;
    res[3] = data;
    return res;
}

uint64_t u8to64(const uint8_t *begin) {
    return static_cast<uint64_t>(*begin) << 56 |
           static_cast<uint64_t>(*(begin + 1)) << 48 |
           static_cast<uint64_t>(*(begin + 2)) << 40 |
           static_cast<uint64_t>(*(begin + 3)) << 32 |
           static_cast<uint64_t>(*(begin + 4)) << 24 |
           static_cast<uint64_t>(*(begin + 5)) << 16 |
           static_cast<uint64_t>(*(begin + 6)) << 8 |
           static_cast<uint64_t>(*(begin + 7));
}

vector<uint8_t> u64to8(uint64_t data) {
    vector<uint8_t> res(8);
    res[0] = data;
    res[1] = data >> 8;
    res[2] = data >> 16;
    res[3] = data >> 24;
    res[4] = data >> 32;
    res[5] = data >> 40;
    res[6] = data >> 48;
    res[7] = data >> 56;
    return res;
}

uint32_t u32rotr(uint32_t n, int d) {
    const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
    d &= mask;
    return (n >> d) | (n << ((-d) & mask));
}

uint32_t u32rotl(uint32_t n, int d) {
    return (n << d) | (n >> (32 - d));
}

vector<uint8_t> random_data(int len) {
    vector<uint8_t> res(len);
    for (int i = 0; i < len; i++) {
        res[i] = (uint8_t) rand();
    }
    return res;
}

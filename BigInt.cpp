//
// Created by anast on 11/14/2020.
//

#include <cassert>
#include "BigInt.h"

template<typename T>
T &getAt(vector<T> &v, size_t pos) {
    if (pos >= v.size()) {
        int x = 5;
    }
    return v.at(pos);
}

template<typename T>
const T &getAt(const vector<T> &v, size_t pos) {
    if (pos >= v.size()) {
        int x = 5;
    }
    return v.at(pos);
}

BigInt::BigInt() {
    number.push_back(0);
}

BigInt::BigInt(long long num) {
    do {
        number.push_back(num % BASE);
        num /= BASE;
    } while (num);
}

BigInt::BigInt(vector<uint8_t> data) {
    while (data.size() % 4 != 0) {
        data.push_back(0);
    }
    this->number.assign(data.size() / 4, 0);
    for (int i = 0; i < data.size() / 4; i++) {
        this->number[i] = u8to32(data.data() + i * 4);
    }
}

bool BigInt::operator==(const BigInt &other) const {
    if (this->number.size() != other.number.size())
        return false;
    for (int i = 0; i < this->number.size(); i++) {
        if (getAt(this->number, i) != getAt(other.number, i))
            return false;
    }
    return true;
}

bool BigInt::operator>(const BigInt &other) const {
    if (this->number.size() != other.number.size())
        return number.size() > other.number.size();
    for (int i = this->number.size() - 1; i >= 0; i--) {
        if (getAt(this->number, i) != getAt(other.number, i))
            return getAt(this->number, i) > getAt(other.number, i);
    }
    return false;
}

bool BigInt::operator<(const BigInt &other) const {
    return other > *this;
}

bool BigInt::operator!=(const BigInt &other) const {
    return !(other == *this);
}


BigInt BigInt::operator+(const BigInt &other) const {
    BigInt res;
    res.number.assign(max(other.number.size(), this->number.size()) + 1, 0);
    for (int i = 0; i < max(other.number.size(), this->number.size()); i++) {
        if (i >= other.number.size()) {
            if (is_poly) {
                getAt(res.number, i) ^= getAt(this->number, i);
            } else {
                getAt(res.number, i) += getAt(this->number, i);
            }
        } else if (i >= this->number.size()) {
            if (is_poly) {
                getAt(res.number, i) ^= getAt(other.number, i);
            } else {
                getAt(res.number, i) += getAt(other.number, i);
            }
        } else {
            if (is_poly) {
                getAt(res.number, i) ^= getAt(other.number, i) ^ getAt(this->number, i);
            } else {
                getAt(res.number, i) += getAt(other.number, i) + getAt(this->number, i);
            }
        }
        getAt(res.number, i + 1) = getAt(res.number, i) / BASE;
        getAt(res.number, i) = getAt(res.number, i) % BASE;
    }
    if (is_poly) {
        res.makePoly();
    }
    res.truncate();
    return res;
}

BigInt BigInt::operator-(const BigInt &other) const {
    BigInt res = *this;
    for (int i = 0; i < res.number.size(); i++) {
        if (i < other.number.size() && getAt(res.number, i) < getAt(other.number, i) || getAt(res.number, i) >= BASE) {
            getAt(res.number, i) += BASE;
            getAt(res.number, i + 1) -= 1;
        }
        if (i < other.number.size()) {
            getAt(res.number, i) -= getAt(other.number, i);
        }
    }
    res.truncate();
    return res;
}

uint64_t poly_mul(uint64_t a, uint64_t b) {
    uint64_t res = 0;
    while (b) {
        if (b & 1)
            res ^= a;
        a = a << 1;
        b >>= 1;
    }
    return res;
}

BigInt BigInt::operator*(const BigInt &other) const {
    BigInt res;
    res.number.assign(this->number.size() + other.number.size(), 0);
    for (int i = 0; i < this->number.size(); i++) {
        for (int j = 0; j < other.number.size(); j++) {
            if (is_poly) {
                getAt(res.number, i + j) ^= poly_mul(getAt(this->number, i), getAt(other.number, j));
                getAt(res.number, i + j + 1) ^= getAt(res.number, i + j) >> BASE_BITS;
                getAt(res.number, i + j) = getAt(res.number, i + j) & (BASE - 1);
            } else {
                getAt(res.number, i + j) += getAt(this->number, i) * getAt(other.number, j);
                getAt(res.number, i + j + 1) += getAt(res.number, i + j) / BASE;
                getAt(res.number, i + j) = getAt(res.number, i + j) % BASE;
            }
        }
    }
    if (is_poly) {
        res.makePoly();
    }
    res.truncate();
    return res;
}

BigInt BigInt::operator/(const BigInt &other) const {
    return DivMod(other).first;
}

BigInt BigInt::operator%(const BigInt &other) const {
    return DivMod(other).second;
}

pair<BigInt, BigInt> BigInt::DivMod(BigInt other) const {

    if (is_poly) {
        if (bit_size() < other.bit_size()) {
            return {0, *this};
        }
        uint64_t c = (bit_size() - other.bit_size());
        BigInt res;
        res.makePoly();
        BigInt tmp = *this;
        for(uint64_t i = 0; i <= c; i++) {
            uint64_t j = c - i;
            if (tmp.bit_size() == bit_size() - i) {
                tmp = tmp ^ (other << j);
                res = res ^ (BigInt(1) << j);
            }
        }
        return {res, tmp};
    }

    assert(other != BigInt(0));
    if (other > *this)
        return {BigInt(0), *this};
    assert(!number.empty());
    if (number.size() == 1) {
        return {number[0] / other.number[0], number[0] % other.number[0]};
    }
    if (other.number.size() == 1) {
        uint64_t carry = 0;
        BigInt res = *this;
        for (size_t i = 0; i < number.size(); i++) {
            size_t j = number.size() - i - 1;
            uint64_t cur = res.number[j] + carry * BASE;
            res.number[j] = cur / other.number[0];
            carry = cur % other.number[0];
        }
        res.truncate();
        return {res, carry};
    }
    BigInt res;
    BigInt dividend = *this;

    uint64_t offset = 0;
    while ((other.GetTop() << offset) <= ((BASE - 1) >> 1)) {
        offset++;
    }

    dividend = dividend << offset;
    other = other << offset;

    BigInt cut;
    cut.number = vector<uint64_t>(dividend.number.end() - other.number.size() + 1, dividend.number.end());
    auto pointer = dividend.number.end() - other.number.size() + 1;
    do {
        pointer--;
        cut.number.insert(cut.number.begin(), *pointer);
        cut.truncate();
        uint64_t digit = 0;
        if (cut.number.size() == other.number.size()) {
            digit = cut.number.back() / other.number.back();
        } else if (cut.number.size() > other.number.size()) {
            digit = cut.GetTop2() / other.number.back();
        }
        if (digit) {
            if (cut < other * digit) {
                digit--;
            }
            if (cut < other * digit) {
                digit--;
            }
            cut = cut - other * digit;
        }
        res.number.insert(res.number.begin(), digit);
    } while (pointer != dividend.number.begin());

    res.truncate();
    cut.truncate();

    return {res, cut >> offset};
}

uint64_t BigInt::GetTop2() const {
    assert(number.size() >= 2);
    return number.back() * BASE + number.at(number.size() - 2);
}

BigInt &BigInt::operator+=(const BigInt &other) {
    *this = *this + other;
    return *this;
}

BigInt &BigInt::operator-=(const BigInt &other) {
    *this = *this - other;
    return *this;
}


BigInt &BigInt::operator*=(const BigInt &other) {
    *this = *this * other;
    return *this;
}


BigInt &BigInt::operator/=(const BigInt &other) {
    *this = *this / other;
    return *this;
}


BigInt BigInt::rand(const BigInt &from, const BigInt &to) {
    BigInt res;
    res.number.assign(to.number.size() + 5, 0);
    for (int i = 0; i < res.number.size(); i++) {
        getAt(res.number, i) = ::rand() % BASE;
    }
    return res % (to - from + 1) + from;
}

BigInt BigInt::pow(const BigInt &n, const BigInt &p) {
    if (p == 0)
        return 1;
    if (p % 2 == 1)
        return pow(n, p - 1) * n;
    else {
        BigInt b = pow(n, p / 2);
        return b * b;
    }
}

BigInt BigInt::mpow(BigInt n, BigInt p, const BigInt &m) {
    BigInt res = 1;
    while (!p.isZero()) {
        if (p.isOdd()) {
            res *= n;
            res = res % m;
        }
        n = n * n;
        n = n % m;
        p = p >> 1;
    }
    return res;
}

BigInt BigInt::gcd(BigInt first, BigInt second) {
    while (second > 0) {
        first = first % second;
        swap(first, second);
    }
    return first;
}

BigInt BigInt::gcdExtended(BigInt a, BigInt b, BigInt &x, BigInt &y, bool &x_neg, bool &y_neg) {
    if (a == 0) {
        x = 0;
        y = 1;
        x_neg = false;
        y_neg = false;
        return b;
    }
    BigInt x1, y1;
    bool x1_neg, y1_neg;
    BigInt gcd = gcdExtended(b % a, a, x1, y1, x1_neg, y1_neg);
    if (x1_neg && !y1_neg) {
        x = y1 + (b / a) * x1;
        y = x1;
        x_neg = false;
        y_neg = true;
    } else if (!x1_neg && y1_neg) {
        x = (b / a) * x1 + y1;
        y = x1;
        x_neg = true;
        y_neg = false;
    } else {
        if ((b / a) * x1 > y1) {
            x = (b / a) * x1 - y1;
            y = x1;
            x_neg = true;
            y_neg = false;
        } else {
            x = y1 - (b / a) * x1;
            y = x1;
            x_neg = false;
            y_neg = false;
        }
    }
    return gcd;
}

BigInt BigInt::inverseModulo(const BigInt &a, const BigInt &m) {
    BigInt x, y;
    bool x_neg, y_neg;
    gcdExtended(a, m, x, y, x_neg, y_neg);
    assert(x < m);
    if (x_neg) {
        return m - x;
    }
    return x;
}

vector<uint8_t> BigInt::tou8() {
    vector<uint8_t> res;
    for (uint64_t block:this->number) {
        vector<uint8_t> temp = u32to8((uint32_t) block);
        res.insert(res.end(), temp.begin(), temp.end());
    }
    return res;
}

BigInt BigInt::operator^(const BigInt &other) const {
    BigInt res;
    BigInt min;
    if (number.size() >= other.number.size()) {
        res = *this;
        min = other;
    } else {
        res = other;
        min = *this;
    }
    for (int i = 0; i < min.number.size(); i++) {
        res.number[i] ^= min.number[i];
    }
    res.truncate();
    return res;
}

BigInt BigInt::operator&(const BigInt &other) const {
    BigInt res;
    BigInt min;
    if (number.size() >= other.number.size()) {
        res = *this;
        min = other;
    } else {
        res = other;
        min = *this;
    }
    for (int i = 0; i < min.number.size(); i++) {
        res.number[i] &= min.number[i];
    }
    res.number.resize(min.number.size());
    res.truncate();
    return res;
}

bool BigInt::isOdd() const {
    return number[0] & 1u;
}

void BigInt::truncate() {
    while (number.size() > 1 && number.back() == 0) {
        number.pop_back();
    }
}

BigInt BigInt::operator<<(uint64_t bits) const {
    BigInt res = *this;

    uint64_t digits_offset = bits / BASE_BITS;
    uint64_t offset = bits % BASE_BITS;

    uint64_t prefix = res.number.back() >> (BASE_BITS - offset);

    size_t prev_size = res.number.size();
    res.number.reserve(res.number.size() + digits_offset + 1);
    res.number.resize(res.number.size() + digits_offset);

    for (size_t i = 0; i + 1 < prev_size; i++) {
        size_t j = prev_size - i - 1;
        res.number[j + digits_offset] =
                ((res.number[j] << offset) | (res.number[j - 1] >> (BASE_BITS - offset))) & (BASE - 1);
    }

    res.number[digits_offset] = (res.number[0] << offset) & (BASE - 1);

    if (prefix) {
        res.number.push_back(prefix);
    }

    for (size_t i = 0; i < digits_offset; i++) {
        res.number[i] = 0;
    }
    res.truncate();
    return res;
}

BigInt BigInt::operator>>(uint64_t bits) const {
    BigInt res = *this;

    uint64_t digits_offset = bits / BASE_BITS;
    uint64_t offset = bits % BASE_BITS;

    if (digits_offset >= res.number.size()) {
        return 0;
    }

    res.number.erase(res.number.begin(), res.number.begin() + digits_offset);
    res.number[0] >>= offset;
    for (size_t i = 0; i + 1 < res.number.size(); i++) {
        res.number[i] |= (res.number[i + 1] << (BASE_BITS - offset)) & (BASE - 1);
        res.number[i + 1] >>= offset;
    }
    res.truncate();
    return res;
}

bool BigInt::isZero() const {
    return number.size() == 1 && number[0] == 0;
}

uint64_t BigInt::lowestDigit() const {
    return number[0];
}

BigInt BigInt::fromHex(const string &hex) {
    BigInt res = 0;
    for (char c : hex) {
        res *= 16;
        if (c >= '0' && c <= '9')
            res += (c - '0');
        else
            res += (c - 'A' + 10);
    }
    return res;
}

void BigInt::makePoly(bool p) {
    is_poly = p;
}

uint64_t BigInt::GetTop() const {
    return number.back();
}

uint64_t BigInt::bit_size() const {
    if (isZero()) {
        return 1;
    }
    uint64_t l = (number.size() - 1) * BASE_BITS;
    uint64_t x = GetTop();
    while(x) {
        x>>=1;
        l++;
    }
    return l;
}








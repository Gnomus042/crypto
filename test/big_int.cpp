//
// Created by anast on 11/14/2020.
//

#include <gtest/gtest.h>
#include "../BigInt.h"

TEST(BigInt, addition) {
    BigInt first(4);
    BigInt second(4);
    EXPECT_EQ(first + second, BigInt(8));

    first = BigInt(44);
    second = BigInt(67);
    EXPECT_EQ(first + second, BigInt(111));

//    first = BigInt(2147483647);
//    second = BigInt(1);
//    EXPECT_EQ(first + second, BigInt("2147483648"));
}

TEST(BigInt, substraction) {
    BigInt first(5);
    BigInt second(2);
    EXPECT_EQ(first - second, BigInt(3));

    first = BigInt(1000);
    second = BigInt(1);
    EXPECT_EQ(first - second, BigInt(999));
}

TEST(BigInt, multiplication) {
    BigInt first(99);
    BigInt second(93);
    EXPECT_EQ(first * second, BigInt(99 * 93));

    first = BigInt(101);
    second = BigInt(99);
    EXPECT_EQ(first * second, BigInt(101 * 99));
}

TEST(BigInt, division) {
    BigInt first(99);
    BigInt second(32);
    EXPECT_EQ(first / second, BigInt(99 / 32));

    first = BigInt(1583);
    second = BigInt(35);
    EXPECT_EQ(first / second, BigInt(1583 / 35));
}

TEST(BigInt, Inverse) {
    BigInt m = 1583;
    BigInt a = 37;
    BigInt b = BigInt::inverseModulo(a, m);
    BigInt res = a*b%m;
    std::cout << res.number[0] << std::endl;
    EXPECT_EQ(a*b%m, BigInt(1));
}

TEST(BigInt, Inverse2) {
    BigInt m = 1583;
    BigInt a = 999;
    BigInt b = BigInt::inverseModulo(a, m);
    EXPECT_EQ(a*b%m, BigInt(1));
}


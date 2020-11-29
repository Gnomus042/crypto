//
// Created by anast on 11/26/2020.
//

#include <gtest/gtest.h>
#include "../Lab5/EllipticCurve.h"

BigInt b = BigInt::fromHex("06973B15095675534C7CF7E64A21BD54EF5DD3B8A0326AA936ECE454D2C");
BigInt x = BigInt::fromHex("03FCDA526B6CDF83BA1118DF35B3C31761D3545F32728D003EEB25EFE96");
BigInt y = BigInt::fromHex("09CA8B57A934C54DEEDA9E54A7BBAD95E3B2E91C54D32BE0B9DF96D8D35");
BigInt n = BigInt::fromHex("1000000000000000000000000000013E974E72F8A6922031D2603CFE0D7");
EllipticCurve curve = EllipticCurve(1, b, 233, {233, 9, 4, 1, 0}, x, y, n);

TEST(EllipticCurve, point_generation) {
    // генерація точки
    auto p = curve.create_point();
    EXPECT_TRUE(curve.check_point(p));
}

TEST(EllipticCurve, add_points) {
    // додавання точок
    auto p = curve.create_point();
    auto q = curve.create_point();
    EXPECT_NE(p, q);
    EXPECT_TRUE(curve.check_point(curve.add_points(p, q)));
}

TEST(EllipticCurve, double_point) {
    // подвоєння точки
    auto p = curve.create_point();
    EXPECT_TRUE(curve.check_point(curve.double_point(p)));
}

TEST(EllipticCurve, time_point) {
    // кратні точки
    auto p = curve.create_point();
    auto p231 = curve.mul_point(p, 231);
    auto p198 = curve.mul_point(p, 198);
    auto p033 = curve.mul_point(p, 33);
    EXPECT_TRUE(curve.check_point(p231));
    EXPECT_TRUE(curve.check_point(curve.add_points(p198, p033)));
    EXPECT_EQ(p231, curve.add_points(p198, p033));
}

TEST(EllipticCurve, signature_correct) {
    // перевірка підпису (результат - підпис верифіковано)
    BigInt message = BigInt::fromHex("FFABX45896FF");
    auto [private_key, public_ley] = EllipticCurveSignature::GenerateKeys(curve);
    auto sign = EllipticCurveSignature::Sign(message, private_key, curve);
    EXPECT_TRUE(EllipticCurveSignature::CheckSign(message, sign, public_ley, curve));
}

TEST(EllipticCurve, signature_incorrect) {
    // перевірка підпису (результат - підпис не верифіковано)
    BigInt message = BigInt::fromHex("FFABX45896FF");
    BigInt message_other = BigInt::fromHex("FFABX45896FA");
    auto [private_key, public_ley] = EllipticCurveSignature::GenerateKeys(curve);
    auto sign = EllipticCurveSignature::Sign(message, private_key, curve);
    EXPECT_FALSE(EllipticCurveSignature::CheckSign(message_other, sign, public_ley, curve));
}

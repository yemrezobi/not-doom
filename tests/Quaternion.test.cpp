#include <cmath>
#include <format>
#include <numbers>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Quaternion.hpp"
#include "Vector3.hpp"

bool almost_equals(double a, double b)
{
    constexpr double epsilon = 0.001;
    return std::fabs(a - b) < epsilon;
}

MATCHER_P(QuaterniondEq, q, std::format("is{} equal to {{{}, {}, {}, {}}}", negation ? " not" : "", q.w, q.x, q.y, q.z)) {
    return almost_equals(arg.w, q.w) &&
        almost_equals(arg.x, q.x) &&
        almost_equals(arg.y, q.y) &&
        almost_equals(arg.z, q.z);
}

MATCHER_P(Vector3dEq, v, std::format("is{} equal to {{{}, {}, {}}}", negation ? " not" : "", v.x, v.y, v.z)) {
    return almost_equals(arg.x, v.x) &&
        almost_equals(arg.y, v.y) &&
        almost_equals(arg.z, v.z);
}

TEST(FromAngleAxisTest, Test1)
{
    const Quaterniond result = Quaterniond::from_axis_angle({0, 0, 1}, std::numbers::pi / 2);
    const Quaterniond expected{0.707106781187, 0, 0, 0.707106781187};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(FromAngleAxisTest, Test2)
{
    const Quaternion result = Quaterniond::from_axis_angle({0.508755913214, -0.351586675739, 0.785846187375}, 3.0 / 5.0 * std::numbers::pi);
    const Quaternion expected{0.587785252293, 0.411592179779, -0.284439595669, 0.635762920551};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(NormalizedTest, DivideByZero)
{
    const Quaterniond q{0, 0, 0, 0};
    const Quaterniond result = q.normalized();
    const Quaterniond expected{0, 0, 0, 0};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(NormalizedTest, Test1)
{
    const Quaterniond q{5.3, 6.32, -0.12, 19.2};
    const Quaterniond result = q.normalized();
    const Quaterniond expected{0.253624285977, 0.302434997618, -0.00574243666364, 0.918789866182};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(InverseTest, Test1)
{
    const Quaterniond q{5.3, 6.32, -0.12, 19.2};
    const Quaterniond result = q.normalized().inverse();
    const Quaterniond expected{0.253624285977, -0.302434997618, 0.00574243666364, -0.918789866182};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(DotTest, Test1)
{
    const Quaterniond q1{5.3, 6.32, -0.12, 19.2};
    const Quaterniond q2{7.1, -2.425, 18.19, 0.01};
    const double result = q1.dot(q2);
    const double expected = 20.3132;
    EXPECT_DOUBLE_EQ(result, expected);
}

TEST(ApplyTest, Test0)
{
    const Quaterniond q{0, 1, 1, 1};
    const Vector3d pos{1, 1, 1};
    const Vector3d result = q.rotate_point(pos);
    const Vector3d expected{1, 1, 1};
    EXPECT_THAT(result.normalized(), Vector3dEq(expected.normalized()));
}

TEST(ApplyTest, Test1)
{
    const Quaterniond q = Quaterniond::from_axis_angle({0, 0, 1}, std::numbers::pi / 2);
    const Vector3d pos{1, 1, 1};
    const Vector3d result = q.rotate_point(pos);
    const Vector3d expected{-1, 1, 1};
    EXPECT_THAT(result, Vector3dEq(expected));
}

TEST(ApplyTest, Test2)
{
    const Quaternion q = Quaterniond::from_axis_angle({0.508755913214, -0.351586675739, 0.785846187375}, 3.0 / 5.0 * std::numbers::pi);
    const Vector3d pos{4.7, 0, 10.3};
    const Vector3d result = q.rotate_point(pos);
    const Vector3d expected{2.0864608014, -6.29671942139, 9.17485726606};
    EXPECT_THAT(result, Vector3dEq(expected));
}

TEST(SlerpTest, Test0)
{
    const Quaterniond q1{0, 0, 1, 0};
    const Quaterniond q2{0, 1, 0, 1};
    const Quaterniond result = q1.normalized().slerp_to(q2.normalized(), 0.5);
    const Quaterniond expected{0, 0.5, 0.707106781187, 0.5};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(SlerpTest, Test1)
{
    const Quaterniond q1{0, 2.34, 8.23, -3.22};
    const Quaterniond q2{0, 3.64, -1.29, 0.98};
    const Quaterniond result = q1.normalized().slerp_to(q2.normalized(), 0.7);
    const Quaterniond expected{0, 0.988141434182, 0.142755337134, 0.0565457316988};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(QuatAdditionTest, Test1)
{
    const Quaterniond q1{5.3, 6.32, -0.12, 19.2};
    const Quaterniond q2{7.1, -2.425, 18.19, 0.01};
    const Quaterniond result = q1 + q2;
    const Quaterniond expected{12.4, 3.895, 18.07, 19.21};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(QuatMultTest, Test1)
{
    const Quaterniond q1{-526.1, -369.2, 144.9, -0.8354};
    const Quaterniond q2{4.8, 5.7, 6.3, -429.2};
    const Quaterniond result = q2 * q1;
    const Quaterniond expected{-1692.26368, 57414.88698, 155846.49178, 228950.00008};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(QuatMultTest, Test2)
{
    const Quaterniond q1{4.8, 5.7, 6.3, -429.2};
    const Quaterniond q2{-526.1, -369.2, 144.9, -0.8354};
    //multiplication is not commutative
    const Quaterniond result = q2 * q1;
    const Quaterniond expected{-1692.26368, -66956.74698, -161084.31178, 222646.22008};
    EXPECT_THAT(result, QuaterniondEq(expected));
}

TEST(QuaternionTest, RotateVec3)
{
    const Vector3d temp{4.12, -7.3, 52.1};
    const Quaterniond q1 = Quaterniond::from_axis_angle(temp.normalized(), std::numbers::pi / 4);
    const Vector3d point{4.7, 0.0, 63.0};
    const Vector3d result = q1.rotate_point(point);
    const Vector3d expected{-1.40840464129, -2.73191675346, 63.1002617048};
    EXPECT_THAT(result, Vector3dEq(expected));
}

#include <numbers>

#include <gtest/gtest.h>
#include <numbers>

#include "Math.hpp"
#include "Quaternion.hpp"
#include "Mat4x4.hpp"

//TODO
//TEST(RotateXTest, NoRotation)
//{
//    Quaternion vec = {1, 1, 1, 0};
//    Mat4x4 mat = math::RotateX(0);
//
//    Quaternion result = mat * vec;
//    Quaternion expected = vec;
//    EXPECT_DOUBLE_EQ(result.x, vec.x);
//    EXPECT_DOUBLE_EQ(result.y, vec.y);
//    EXPECT_DOUBLE_EQ(result.z, vec.z);
//    EXPECT_DOUBLE_EQ(result.w, vec.w);
//}
//
//TEST(RotateXTest, HalfPi)
//{
//    Quaternion vec = {1, 1, 1, 0};
//    Mat4x4 mat = math::RotateX(std::numbers::pi / 2);
//
//    Quaternion result = mat * vec;
//    Quaternion expected = {1, 1, -1, 0};
//    EXPECT_DOUBLE_EQ(result.x, vec.x);
//    EXPECT_DOUBLE_EQ(result.y, vec.y);
//    EXPECT_DOUBLE_EQ(result.z, vec.z);
//    EXPECT_DOUBLE_EQ(result.w, vec.w);
//}
//
//TEST(RotateXTest, Pi)
//{
//    Quaternion vec = {1, -1, -1, 0};
//    Mat4x4 mat = math::RotateX(std::numbers::pi);
//
//    Quaternion result = mat * vec;
//    Quaternion expected = {-1, -1, 1, 0};
//    EXPECT_DOUBLE_EQ(result.x, vec.x);
//    EXPECT_DOUBLE_EQ(result.y, vec.y);
//    EXPECT_DOUBLE_EQ(result.z, vec.z);
//    EXPECT_DOUBLE_EQ(result.w, vec.w);
//}
//
//TEST(RotateYTest, NoRotation)
//{
//    Quaternion vec = {1, 1, 1, 0};
//    Mat4x4 mat = math::RotateY(0);
//
//    Quaternion result = mat * vec;
//    Quaternion expected = vec;
//    EXPECT_DOUBLE_EQ(result.x, vec.x);
//    EXPECT_DOUBLE_EQ(result.y, vec.y);
//    EXPECT_DOUBLE_EQ(result.z, vec.z);
//    EXPECT_DOUBLE_EQ(result.w, vec.w);
//}
//
//TEST(RotateYTest, HalfPi)
//{
//    Quaternion vec = {1, 1, 1, 0};
//    Mat4x4 mat = math::RotateY(std::numbers::pi / 2);
//
//    Quaternion result = mat * vec;
//    Quaternion expected = {-1, 1, 1, 0};
//    EXPECT_DOUBLE_EQ(result.x, vec.x);
//    EXPECT_DOUBLE_EQ(result.y, vec.y);
//    EXPECT_DOUBLE_EQ(result.z, vec.z);
//    EXPECT_DOUBLE_EQ(result.w, vec.w);
//}
//
//TEST(RotateYTest, Pi)
//{
//    Quaternion vec = {1, 1, 1, 0};
//    Mat4x4 mat = math::RotateY(std::numbers::pi);
//
//    Quaternion result = mat * vec;
//    Quaternion expected = {-1, 1, -1, 0};
//    EXPECT_DOUBLE_EQ(result.x, vec.x);
//    EXPECT_DOUBLE_EQ(result.y, vec.y);
//    EXPECT_DOUBLE_EQ(result.z, vec.z);
//    EXPECT_DOUBLE_EQ(result.w, vec.w);
//}
//
//TEST(RotateZTest, NoRotation)
//{
//    Quaternion vec = {1, 1, 1, 0};
//    Mat4x4 mat = math::RotateZ(0);
//
//    Quaternion result = mat * vec;
//    Quaternion expected = vec;
//    EXPECT_DOUBLE_EQ(result.x, vec.x);
//    EXPECT_DOUBLE_EQ(result.y, vec.y);
//    EXPECT_DOUBLE_EQ(result.z, vec.z);
//    EXPECT_DOUBLE_EQ(result.w, vec.w);
//}
//
//TEST(RotateZTest, HalfPi)
//{
//    Quaternion vec = {1, 1, 1, 0};
//    Mat4x4 mat = math::RotateZ(std::numbers::pi / 2);
//
//    Quaternion result = mat * vec;
//    Quaternion expected = {1, -1, 1, 0};
//    EXPECT_DOUBLE_EQ(result.x, vec.x);
//    EXPECT_DOUBLE_EQ(result.y, vec.y);
//    EXPECT_DOUBLE_EQ(result.z, vec.z);
//    EXPECT_DOUBLE_EQ(result.w, vec.w);
//}
//
//TEST(RotateZTest, Pi)
//{
//    Quaternion vec = {1, 1, 1, 0};
//    Mat4x4 mat = math::RotateZ(std::numbers::pi);
//
//    Quaternion result = mat * vec;
//    Quaternion expected = {-1, -1, 1, 0};
//    EXPECT_DOUBLE_EQ(result.x, vec.x);
//    EXPECT_DOUBLE_EQ(result.y, vec.y);
//    EXPECT_DOUBLE_EQ(result.z, vec.z);
//    EXPECT_DOUBLE_EQ(result.w, vec.w);
//}

#include <cmath>
#include <format>

#include <gmock/gmock.h> // IWYU pragma: keep
// IWYU pragma: no_include "gmock/gmock.h" // for MakePredicateFormatterFromMatcher, GMOCK_P...
#include <gtest/gtest.h>
// IWYU pragma: no_include <memory> // for allocator
// IWYU pragma: no_include "gtest/gtest.h" // for DeathTest, Message, TestPartResult

#include "Matrix4x4.hpp"
#include "Quaternion.hpp"
#include "Vector3.hpp"

inline bool almost_equals(double a, double b)
{
    constexpr double epsilon = 0.005;
    return std::fabs(a - b) < epsilon;
}

MATCHER_P(Matrix4x4dEq, mat,
    std::format("is{} equal to\n"
                "[{:> 10.3f}, {:> 10.3f}, {:> 10.3f}, {:> 10.3f}]\n"
                "[{:> 10.3f}, {:> 10.3f}, {:> 10.3f}, {:> 10.3f}]\n"
                "[{:> 10.3f}, {:> 10.3f}, {:> 10.3f}, {:> 10.3f}]\n"
                "[{:> 10.3f}, {:> 10.3f}, {:> 10.3f}, {:> 10.3f}]",
        negation ? " not" : "", mat.data[0], mat.data[1], mat.data[2], mat.data[3], mat.data[4],
        mat.data[5], mat.data[6], mat.data[7], mat.data[8], mat.data[9], mat.data[10], mat.data[11], mat.data[12],
        mat.data[13], mat.data[14], mat.data[15]))
{
    for (int i = 0; i < 16; i++) {
        if (!almost_equals(mat.data[i], arg.data[i])) {
            return false;
        }
    }
    return true;
}

MATCHER_P(Vector3dEq, v, std::format("is{} equal to {{{}, {}, {}}}", negation ? " not" : "", v.x, v.y, v.z))
{
    return almost_equals(arg.x, v.x) && almost_equals(arg.y, v.y) && almost_equals(arg.z, v.z);
}

TEST(MatrixAccessTest, Test1) {
    Matrix4x4d mat{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const double replacement = -512.98;
    mat[3] = replacement;
    EXPECT_THAT(mat[3], testing::Eq(replacement));
}

TEST(MatrixAccessTest, Test2) {
    const Matrix4x4d mat{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const double result = mat.at(2, 1);
    const double expected = -517.37;
    EXPECT_THAT(result, testing::Eq(expected));
}

TEST(MatrixAccessTest, Test3) {
    const Matrix4x4d mat{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const double result = mat.at(3, 3);
    const double expected = 66.20;
    EXPECT_THAT(result, testing::Eq(expected));
}

TEST(MatrixScalarAdditionTest, Addition) {
    const Matrix4x4d mat1{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const Matrix4x4d result = mat1 + 76.28;
    const Matrix4x4d expected{-296.37, 877.24, -714.46, 221.01, 1098.72, 72.93, -545.15, 575.27, 809.97, -441.09, -428.30, 1019.08, -408.69, -71.75, 1061.65, 142.48};
    EXPECT_THAT(result, Matrix4x4dEq(expected));
}

TEST(MatrixScalarAdditionTest, Subtraction) {
    const Matrix4x4d mat1{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const Matrix4x4d result = mat1 - 76.28;
    const Matrix4x4d expected{-448.93, 724.68, -867.02, 68.45, 946.16, -79.63, -697.71, 422.71, 657.41, -593.65, -580.86, 866.52, -561.25, -224.31, 909.09, -10.08};
    EXPECT_THAT(result, Matrix4x4dEq(expected));
}

TEST(MatrixScalarMultTest, Multiplication) {
    const Matrix4x4d mat1{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const Matrix4x4d result = mat1 * 2.54;
    const Matrix4x4d expected{-946.53, 2034.44, -2008.48, 367.61, 2597.00, -8.51, -1578.43, 1267.43, 1863.57, -1314.12, -1281.63, 2394.71, -1231.82, -376.00, 2502.84, 168.15};
    EXPECT_THAT(result, Matrix4x4dEq(expected));
}

TEST(MatrixScalarMultTest, Division) {
    const Matrix4x4d mat1{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const Matrix4x4d result = mat1 / 2.54;
    const Matrix4x4d expected{-146.71, 315.34, -311.31, 56.98, 402.54, -1.32, -244.66, 196.45, 288.85, -203.69, -198.65, 371.18, -190.93, -58.28, 387.94, 26.06};
    EXPECT_THAT(result, Matrix4x4dEq(expected));
}

TEST(MatrixMultTest, Identity)
{
    const Matrix4x4d mat1{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const Matrix4x4d result = mat1 * Matrix4x4d::identity();
    EXPECT_THAT(result, Matrix4x4dEq(mat1));
}

TEST(MatrixMultTest, ZeroMatrices)
{
    const Matrix4x4d mat1{};
    const Matrix4x4d mat2{};
    const Matrix4x4d result = mat1 * mat2;
    EXPECT_THAT(result, Matrix4x4dEq(mat2));
}

TEST(MatrixMultTest, ZeroMatrix)
{
    const Matrix4x4d mat1{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const Matrix4x4d mat2{};
    const Matrix4x4d result = mat1 * mat2;
    EXPECT_THAT(result, Matrix4x4dEq(mat2));
}

TEST(MatrixMultTest, Test1)
{
    const Matrix4x4d mat1{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const Matrix4x4d mat2{346.48, -253.20, 766.78, -68.55, 353.64, 937.99, -485.22, -972.35, -610.42, 550.54, 734.42, -942.18, -531.71, -246.17, -751.73, 444.67};
    const Matrix4x4d result = mat1 * mat2;
    const Matrix4x4d expected{559864.845, 374685.267, -1363915.532, 56108.204, 467085.645, -726982.515, -45884.343, 740553.911, -122044.280, -1180938.743, -265687.598, 1347410.330, -857070.492, 510132.890, 373872.729, -721777.089};
    EXPECT_THAT(result, Matrix4x4dEq(expected));
}

TEST(MatrixFromQuaternionTest, ZeroQuaternion)
{
    const Quaterniond quaternion{};
    const Matrix4x4d result = Matrix4x4d::from_quaternion(quaternion);
    const Matrix4x4d expected = Matrix4x4d::identity();
    EXPECT_THAT(result, Matrix4x4dEq(expected));
}

TEST(MatrixFromQuaternionTest, IdentityQuaternion)
{
    const Quaterniond quaternion = Quaterniond::identity();
    const Matrix4x4d result = Matrix4x4d::from_quaternion(quaternion);
    const Matrix4x4d expected = Matrix4x4d::identity();
    EXPECT_THAT(result, Matrix4x4dEq(expected));
}

TEST(MatrixFromQuaternionTest, Test1)
{
    const Quaterniond quaternion = Quaterniond{2.45, -7.12, 12.6, -6.42}.normalized();
    const Matrix4x4d result = Matrix4x4d::from_quaternion(quaternion);
    const Matrix4x4d expected{-0.5582174, -0.5764760,  0.5967150, 0, -0.8215969, 0.2838305, -0.4943872, 0, 0.1156365, -0.7662348, -0.6320700, 0, 0, 0, 0, 1};
    EXPECT_THAT(result, Matrix4x4dEq(expected));
}

TEST(MatrixCreateProjectionMatrix, Test1)
{
    const Matrix4x4d result = Matrix4x4d::perspective_matrix(6.0, 24.0, 16.0 / 9.0, 70.0);
    const Matrix4x4d expected{
        1.18717, 0, 0, 0,
        0, 2.11053, 0, 0,
        0, 0, 1.33333, -8,
        0, 0, 1, 0
    };
    EXPECT_THAT(result, Matrix4x4dEq(expected));
}

TEST(MatrixRotatePoint, Zeros)
{
    const Matrix4x4d rotation_matrix{};
    const Vector3d point{};
    const Vector3d result = rotation_matrix.rotate_point(point);
    const Vector3d expected{};
    EXPECT_THAT(result, Vector3dEq(expected));
}

TEST(MatrixRotatePoint, ZeroVector)
{
    const Matrix4x4d rotation_matrix{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const Vector3d point{};
    const Vector3d result = rotation_matrix.rotate_point(point);
    const Vector3d expected{2.186, 7.537, 14.241};
    EXPECT_THAT(result, Vector3dEq(expected));
}

TEST(MatrixRotatePoint, ZeroMatrix)
{
    const Matrix4x4d rotation_matrix{};
    const Vector3d point{2.64, -28.54, 28.87};
    const Vector3d result = rotation_matrix.rotate_point(point);
    const Vector3d expected{};
    EXPECT_THAT(result, Vector3dEq(expected));
}

TEST(MatrixRotatePoint, IdentityMatrix)
{
    const Matrix4x4d rotation_matrix = Matrix4x4d::identity();
    const Vector3d point{2.64, -28.54, 28.87};
    const Vector3d result = rotation_matrix.rotate_point(point);
    EXPECT_THAT(result, Vector3dEq(point));
}

TEST(MatrixRotatePoint, Test1)
{
    const Matrix4x4d rotation_matrix{-372.65, 800.96, -790.74, 144.73, 1022.44, -3.35, -621.43, 498.99, 733.69, -517.37, -504.58, 942.80, -484.97, -148.03, 985.37, 66.20};
    const Vector3d point{2.64, -28.54, 28.87};
    const Vector3d result = rotation_matrix.rotate_point(point);
    const Vector3d expected{-1.479, -0.465, 0.097};
    EXPECT_THAT(result, Vector3dEq(expected));
}

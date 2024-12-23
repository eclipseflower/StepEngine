#include <gtest/gtest.h>
#include "Runtime/Math/Matrix4x4.h"

TEST(Matrix4x4Test, Translate) {
    StepEngine::Matrix4x4 m;
    StepEngine::Vector3 v1(1.0f, 2.0f, 3.0f);
    m.SetTranslation(v1);
    StepEngine::Vector3 v2 = m.GetTranslation();
    EXPECT_EQ(v1.x, v2.x);
    EXPECT_EQ(v1.y, v2.y);
    EXPECT_EQ(v1.z, v2.z);

    StepEngine::Vector3 v3(4.0f, 5.0f, 6.0f);
    StepEngine::Vector3 v4 = m.MultiplyPoint(v3);
    EXPECT_EQ(v3.x + v1.x, v4.x);
    EXPECT_EQ(v3.y + v1.y, v4.y);
    EXPECT_EQ(v3.z + v1.z, v4.z);
}

TEST(Matrix4x4Test, Scale) {
    StepEngine::Matrix4x4 m;
    StepEngine::Vector3 v1(1.0f, 2.0f, 3.0f);
    m.SetScale(v1);
    StepEngine::Vector3 v2 = m.GetScale();
    EXPECT_EQ(v1.x, v2.x);
    EXPECT_EQ(v1.y, v2.y);
    EXPECT_EQ(v1.z, v2.z);

    StepEngine::Vector3 v3(4.0f, 5.0f, 6.0f);
    StepEngine::Vector3 v4 = m.MultiplyVector(v3);
    EXPECT_EQ(v3.x * v1.x, v4.x);
    EXPECT_EQ(v3.y * v1.y, v4.y);
    EXPECT_EQ(v3.z * v1.z, v4.z);
}

TEST(Matrix4x4Test, Rotate) {
    StepEngine::Matrix4x4 m;
    StepEngine::Vector3 v1(1.0f, 2.0f, 3.0f);
    m.SetRotation(v1);
    StepEngine::Vector3 v2 = m.GetRotation();
    EXPECT_EQ(v1.x, v2.x);
    EXPECT_EQ(v1.y, v2.y);
    EXPECT_EQ(v1.z, v2.z);

    StepEngine::Vector3 v3(4.0f, 5.0f, 6.0f);
    StepEngine::Vector3 v4 = m.MultiplyVector(v3);
    EXPECT_EQ(v3.x, v4.x);
    EXPECT_EQ(v3.y, v4.y);
    EXPECT_EQ(v3.z, v4.z);
}

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

}

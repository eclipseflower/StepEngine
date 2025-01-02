#include <gtest/gtest.h>
#include "Runtime/Math/Vector3.h"
#include "Runtime/Math/MathCommon.h"

TEST(Vector3Test, Lerp)
{
    StepEngine::Vector3 v1(1.0f, 2.0f, 3.0f);
    StepEngine::Vector3 v2(4.0f, 5.0f, 6.0f);
    StepEngine::Vector3 v3 = StepEngine::Lerp(v1, v2, 0.5f);

    EXPECT_EQ(2.5f, v3.x);
    EXPECT_EQ(3.5f, v3.y);
    EXPECT_EQ(4.5f, v3.z);
}

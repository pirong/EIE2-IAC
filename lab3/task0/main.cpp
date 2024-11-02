#include "gtest/gtest.h"

int add(int a, int b) { return a + b; }

class TestAdd : public ::testing::Test
{
    void SetUp() override
    {
        // Runs before each test
    }

    void TearDown() override
    {
        // Runs after each test
    }
};

// Don't worry about the syntax here, the TEST_F macro is very complicated.
// Just know that this is how you create a test case.
TEST_F(TestAdd, AddTest)
{
    EXPECT_EQ(add(2, 4), 6);
}

TEST_F(TestAdd, AddTest2)
{
    EXPECT_EQ(add(3, 7), 9);
}

int main(int argc, char **argv)
{
    // Standard Google Test main function
    testing::InitGoogleTest(&argc, argv);
    auto res = RUN_ALL_TESTS();
    return res;
}
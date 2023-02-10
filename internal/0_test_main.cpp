/**
 * @file test_main.cpp
 * @author Thanh Phong Ha (jaden0747@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Example, ExampleTest)
{
    EXPECT_EQ(0 ,0);
}
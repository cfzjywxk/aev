//
// Created by ray on 16-7-16.
//

#include <iostream>
#include <gtest/gtest.h>
#include "../src/sampleClass.h"

class testClass : public testing::Test {

};

TEST_F(testClass, Test1)
{
  sampleClass obj;
  EXPECT_EQ(1, obj.getPlusOne());
  sampleClass obj2(10);
  EXPECT_EQ(11, obj2.getPlusOne());
}

int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

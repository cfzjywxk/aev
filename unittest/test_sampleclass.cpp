//
// Created by ray on 16-7-16.
//

#include "deps/tbsys/src/tblog.h"
#include <gtest/gtest.h>
#include "sample_class.h"

class testClass : public testing::Test {
  virtual void SetUp() {
    TBSYS_LOG(INFO, "testClass setup function executed");
  }
};

TEST_F(testClass, Test1) {
  sampleClass obj;
  EXPECT_EQ(1, obj.getPlusOne());

  TBSYS_LOG(INFO, "testClass first test finished");
  sampleClass obj2(10);
  EXPECT_EQ(11, obj2.getPlusOne());
  TBSYS_LOG(INFO, "testClass second test finished");
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

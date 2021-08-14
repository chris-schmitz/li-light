#include "PixelRange.h"
#include <unity.h>

void test_forward_incrementation(void)
{
  PixelRange range = PixelRange(1, 5, 10);
  uint8_t currentIndex = range.getCurrentIndex();

  TEST_ASSERT_EQUAL(5, currentIndex);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_forward_incrementation);
  UNITY_END();
  return 0;
}
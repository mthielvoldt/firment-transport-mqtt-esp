#include <CppUTest/TestHarness.h>
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "test_shared.h"
#include <signal.h> // for debugging tests: raise(SIGINT);
}

TEST_GROUP(fmt_transport)
{
  void setup()
  {
  };
  void teardown()
  {
    mock().checkExpectations();
    mock().clear();
  };
};

TEST(fmt_transport, setup_succeeds)
{
  // CHecks just setup.
}

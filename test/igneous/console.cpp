#include <gtest/gtest.h>

#include <igneous/console/console.hpp>

namespace {
TEST(Parse, Empty)
{
	EXPECT_NO_FATAL_FAILURE(igneous::Console::getInstance().execute(""));
}
} // end namespace

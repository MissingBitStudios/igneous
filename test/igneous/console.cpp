#include <gtest/gtest.h>

#include <igneous/console.hpp>

namespace {
TEST(Parse, Empty)
{
	EXPECT_NO_FATAL_FAILURE(
		igneous::console::init();
		igneous::console::execute("");
	);
}
} // end namespace

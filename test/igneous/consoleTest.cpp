#include <gtest/gtest.h>

#include <igneous/console.hpp>

namespace {
TEST(Parse, Empty)
{
	EXPECT_NO_FATAL_FAILURE(igneous::Console::getInstance().parse(""));
}
} // end namespace

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

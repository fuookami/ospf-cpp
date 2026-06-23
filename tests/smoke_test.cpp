#include <gtest/gtest.h>
#include <expected>
#include <string>

TEST(Smoke, Hello) {
    EXPECT_EQ(1 + 1, 2);
}

TEST(Smoke, Cpp23Features) {
    // Verify C++23 std::expected is available
    std::expected<int, std::string> exp = 42;
    EXPECT_TRUE(exp.has_value());
    EXPECT_EQ(exp.value(), 42);
}

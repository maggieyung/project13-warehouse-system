#include <gtest/gtest.h>
#include "product.h"

TEST(ProductTest, CreatesValidProduct)
{
    ASSERT_TRUE(isValidProduct("KB001", "Keyboard"));

    Product product("KB001", "Keyboard");

    EXPECT_EQ(product.getSku(), "KB001");
    EXPECT_EQ(product.getName(), "Keyboard");
}

TEST(ProductTest, RejectsEmptySku)
{
    EXPECT_FALSE(isValidProduct("", "Keyboard"));
}

TEST(ProductTest, RejectsEmptyName)
{
    EXPECT_FALSE(isValidProduct("KB001", ""));
}

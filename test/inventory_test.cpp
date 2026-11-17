#include "inventory.h"
#include <gtest/gtest.h>

// Test 1: Adds 10 keyboards
TEST(InventoryTest, AddsStockSuccessfully)
{
    Inventory inventory;

    EXPECT_TRUE(inventory.addStock("KEYBOARD001", 10));
    EXPECT_EQ(inventory.quantity("KEYBOARD001"), 10);
}

// Test 2: Adds 5 more keyboards
TEST(InventoryTest, AddsMoreStock)
{
    Inventory inventory;

    inventory.addStock("KEYBOARD001", 10);
    EXPECT_TRUE(inventory.addStock("KEYBOARD001", 5));

    EXPECT_EQ(inventory.quantity("KEYBOARD001"), 15);
}

// Test 3: Different products
TEST(InventoryTest, TracksDifferentProducts)
{
    Inventory inventory;

    inventory.addStock("KEYBOARD001", 10);
    inventory.addStock("MOUSE001", 20);

    EXPECT_EQ(inventory.quantity("KEYBOARD001"), 10);
    EXPECT_EQ(inventory.quantity("MOUSE001"), 20);
}

// Test 4: Rejects the negative stock
TEST(InventoryTest, RejectsNegativeStock)
{
    Inventory inventory;

    inventory.addStock("KEYBOARD001", 10);

    EXPECT_FALSE(inventory.addStock("KEYBOARD001", -5));
    EXPECT_EQ(inventory.quantity("KEYBOARD001"), 10);
}

TEST(InventoryTest, RejectsReservationWhenStockIsInsufficient)
{
    Inventory inventory;
    inventory.addStock("KEYBOARD001", 10);

    EXPECT_FALSE(inventory.reserveStock("KEYBOARD001", 11));
    EXPECT_EQ(inventory.quantity("KEYBOARD001"), 10);
}

TEST(InventoryTest, ReservesAvailableStock)
{
    Inventory inventory;
    inventory.addStock("KEYBOARD001", 10);

    EXPECT_TRUE(inventory.reserveStock("KEYBOARD001", 4));
    EXPECT_EQ(inventory.quantity("KEYBOARD001"), 6);
}

// Test 5: Unknown product
TEST(InventoryTest, UnknownProductHasZeroStock)
{
    Inventory inventory;

    EXPECT_EQ(inventory.quantity("UNKNOWN"), 0);
}
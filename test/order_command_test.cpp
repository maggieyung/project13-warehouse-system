#include <gtest/gtest.h>
#include <string>
#include "OrderBook.h"
#include "OrderCommand.h"

namespace {

// Same sample stock as src/main.cpp, so the tests show what a user sees.
OrderBook::StockMap sampleStock()
{
    return {{"SKU-100", 10}, {"SKU-200", 5}};
}

bool contains(const std::string& text, const std::string& part)
{
    return text.find(part) != std::string::npos;
}

} // namespace

// AC4 happy path
TEST(OrderCommandTest, ParsesValidCommand)
{
    ParsedOrder parsed;
    std::string error;

    ASSERT_TRUE(parseOrderCommand("order C1 SKU-100:3 SKU-200:2", parsed, error)) << error;
    EXPECT_EQ(parsed.customerId, "C1");
    ASSERT_EQ(parsed.lines.size(), 2u);
    EXPECT_EQ(parsed.lines[0].sku, "SKU-100");
    EXPECT_EQ(parsed.lines[0].quantity, 3);
    EXPECT_EQ(parsed.lines[1].sku, "SKU-200");
    EXPECT_EQ(parsed.lines[1].quantity, 2);
}

// AC4
TEST(OrderCommandTest, RejectsMalformedQuantity)
{
    for (const std::string bad : {"SKU-100:x", "SKU-100:0", "SKU-100:-1", "SKU-100:3abc", "SKU-100:"}) {
        ParsedOrder parsed;
        std::string error;
        EXPECT_FALSE(parseOrderCommand("order C1 " + bad, parsed, error)) << bad;
        EXPECT_FALSE(error.empty()) << bad;
    }
}

// AC4
TEST(OrderCommandTest, RejectsMissingPartsAndCreatesNothing)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();

    for (const std::string bad : {"order", "order C1", "order C1 SKU-100", "order C1 :3"}) {
        std::string output = runOrderCommand(book, stock, bad);
        EXPECT_TRUE(contains(output, "Error:")) << bad << " -> " << output;
    }
    EXPECT_EQ(book.size(), 0u);
    EXPECT_EQ(stock, sampleStock());
}

// AC1 end to end
TEST(OrderCommandTest, ValidOrderReservesStock)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();

    std::string output = runOrderCommand(book, stock, "order C1 SKU-100:3 SKU-200:2");

    EXPECT_TRUE(contains(output, "Order 1 created for C1")) << output;
    EXPECT_TRUE(contains(output, "SKU-100 x3")) << output;
    EXPECT_EQ(stock["SKU-100"], 7);
    EXPECT_EQ(stock["SKU-200"], 3);
    EXPECT_EQ(book.size(), 1u);
}

// AC2 business rule: one short line rejects the whole order
TEST(OrderCommandTest, InsufficientStockRejectsWholeOrder)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();

    std::string output = runOrderCommand(book, stock, "order C2 SKU-100:3 SKU-200:99");

    EXPECT_TRUE(contains(output, "Rejected: insufficient stock for SKU-200")) << output;
    EXPECT_FALSE(contains(output, "SKU-100")) << output;
    EXPECT_EQ(stock["SKU-100"], 10);
    EXPECT_EQ(stock["SKU-200"], 5);
    EXPECT_EQ(book.size(), 0u);
}

// AC3
TEST(OrderCommandTest, UnknownSkuIsRejected)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();

    std::string output = runOrderCommand(book, stock, "order C3 SKU-999:1");

    EXPECT_TRUE(contains(output, "Rejected: unknown SKU SKU-999")) << output;
    EXPECT_EQ(stock, sampleStock());
    EXPECT_EQ(book.size(), 0u);
}

TEST(OrderCommandTest, OtherInputIsUnknownCommand)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();

    EXPECT_TRUE(contains(runOrderCommand(book, stock, "ship 1"), "Unknown command"));
    EXPECT_TRUE(contains(runOrderCommand(book, stock, ""), "Unknown command"));
}

TEST(OrderCommandTest, FulfillCommandMarksOrderFulfilled)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();

    ASSERT_TRUE(contains(runOrderCommand(book, stock, "order C1 SKU-100:3"),
                         "Order 1 created"));
    EXPECT_EQ(runOrderCommand(book, stock, "fulfill 1"), "Order 1 marked fulfilled.");
    ASSERT_NE(book.findOrder(1), nullptr);
    EXPECT_EQ(book.findOrder(1)->status(), OrderStatus::Fulfilled);
}

TEST(OrderCommandTest, FulfillCommandReportsUnknownOrder)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();

    EXPECT_EQ(runOrderCommand(book, stock, "fulfill 99"), "Order 99 was not found.");
}

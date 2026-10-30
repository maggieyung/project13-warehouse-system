#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "Order.h"
#include "OrderBook.h"

namespace {

OrderBook::StockMap sampleStock()
{
    return {{"SKU-100", 10}, {"SKU-200", 5}};
}

} // namespace

TEST(OrderBookTest, ValidOrderIsStoredAndReservesStock)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();
    int newId = 0;

    OrderResult result = book.createOrder("C1", {{"SKU-100", 3}, {"SKU-200", 2}}, stock, newId);

    EXPECT_EQ(result, OrderResult::Ok);
    EXPECT_EQ(newId, 1);
    EXPECT_EQ(stock["SKU-100"], 7);
    EXPECT_EQ(stock["SKU-200"], 3);
    ASSERT_EQ(book.size(), 1u);
    const Order* order = book.findOrder(newId);
    ASSERT_NE(order, nullptr);
    EXPECT_EQ(order->customerId(), "C1");
    EXPECT_EQ(order->lines().size(), 2u);
}

TEST(OrderBookTest, OrderIdsAreSequential)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();
    int firstId = 0;
    int secondId = 0;

    book.createOrder("C1", {{"SKU-100", 1}}, stock, firstId);
    book.createOrder("C2", {{"SKU-100", 1}}, stock, secondId);

    EXPECT_EQ(firstId, 1);
    EXPECT_EQ(secondId, 2);
}

TEST(OrderBookTest, OrderingExactlyAvailableStockSucceeds)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();
    int newId = 0;

    EXPECT_EQ(book.createOrder("C1", {{"SKU-200", 5}}, stock, newId), OrderResult::Ok);
    EXPECT_EQ(stock["SKU-200"], 0);
}

TEST(OrderBookTest, OneShortLineRejectsWholeOrderAndKeepsStock)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();
    int newId = 0;

    OrderResult result = book.createOrder("C1", {{"SKU-100", 3}, {"SKU-200", 6}}, stock, newId);

    EXPECT_EQ(result, OrderResult::InsufficientStock);
    EXPECT_EQ(stock, sampleStock());
    EXPECT_EQ(book.size(), 0u);
}

TEST(OrderBookTest, RejectedOrderDoesNotUseAnId)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();
    int newId = 0;

    book.createOrder("C1", {{"SKU-200", 99}}, stock, newId);
    book.createOrder("C2", {{"SKU-200", 1}}, stock, newId);

    EXPECT_EQ(newId, 1);
}

TEST(OrderBookTest, UnknownSkuIsRejected)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();
    int newId = 0;

    EXPECT_EQ(book.createOrder("C1", {{"SKU-999", 1}}, stock, newId), OrderResult::UnknownSku);
    EXPECT_EQ(stock, sampleStock());
    EXPECT_EQ(book.size(), 0u);
}

TEST(OrderBookTest, InvalidInputIsRejected)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();
    int newId = 0;

    EXPECT_EQ(book.createOrder("", {{"SKU-100", 1}}, stock, newId), OrderResult::MissingCustomer);
    EXPECT_EQ(book.createOrder("C1", {}, stock, newId), OrderResult::EmptyOrder);
    EXPECT_EQ(book.createOrder("C1", {{"SKU-100", 0}}, stock, newId), OrderResult::InvalidQuantity);
    EXPECT_EQ(book.createOrder("C1", {{"SKU-100", -2}}, stock, newId), OrderResult::InvalidQuantity);
    EXPECT_EQ(stock, sampleStock());
    EXPECT_EQ(book.size(), 0u);
}

TEST(OrderBookTest, DuplicateSkusAreCombinedBeforeStockCheck)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();
    int newId = 0;

    // 6 + 6 = 12 is more than the 10 available, even though each line alone fits.
    EXPECT_EQ(book.createOrder("C1", {{"SKU-100", 6}, {"SKU-100", 6}}, stock, newId),
              OrderResult::InsufficientStock);
    EXPECT_EQ(stock, sampleStock());

    EXPECT_EQ(book.createOrder("C1", {{"SKU-100", 2}, {"SKU-100", 3}}, stock, newId),
              OrderResult::Ok);
    EXPECT_EQ(stock["SKU-100"], 5);
    const Order* order = book.findOrder(newId);
    ASSERT_NE(order, nullptr);
    ASSERT_EQ(order->lines().size(), 1u);
    EXPECT_EQ(order->lines()[0].quantity, 5);
}

TEST(OrderBookTest, FindShortagesListsOnlyShortSkus)
{
    OrderBook book;
    OrderBook::StockMap stock = sampleStock();

    std::vector<std::string> shortages =
        book.findShortages({{"SKU-100", 10}, {"SKU-200", 6}}, stock);

    EXPECT_EQ(shortages, std::vector<std::string>({"SKU-200"}));
}

TEST(OrderTest, MergeDuplicateLinesKeepsFirstSeenOrder)
{
    std::vector<OrderLine> merged =
        mergeDuplicateLines({{"SKU-200", 1}, {"SKU-100", 2}, {"SKU-200", 3}});

    ASSERT_EQ(merged.size(), 2u);
    EXPECT_EQ(merged[0].sku, "SKU-200");
    EXPECT_EQ(merged[0].quantity, 4);
    EXPECT_EQ(merged[1].sku, "SKU-100");
    EXPECT_EQ(merged[1].quantity, 2);
}

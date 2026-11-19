#include "OrderBook.h"

std::string toString(OrderResult result)
{
    switch (result) {
    case OrderResult::Ok:
        return "ok";
    case OrderResult::MissingCustomer:
        return "missing customer id";
    case OrderResult::EmptyOrder:
        return "order has no lines";
    case OrderResult::InvalidQuantity:
        return "quantity must be greater than 0";
    case OrderResult::UnknownSku:
        return "unknown SKU";
    case OrderResult::InsufficientStock:
        return "insufficient stock";
    }
    return "unknown result";
}

OrderResult OrderBook::createOrder(const std::string& customerId,
                                   const std::vector<OrderLine>& lines,
                                   StockMap& stock, int& newId)
{
    if (customerId.empty()) {
        return OrderResult::MissingCustomer;
    }
    if (lines.empty()) {
        return OrderResult::EmptyOrder;
    }
    for (const OrderLine& line : lines) {
        if (line.quantity <= 0) {
            return OrderResult::InvalidQuantity;
        }
    }

    std::vector<OrderLine> merged = mergeDuplicateLines(lines);
    if (!findUnknownSkus(merged, stock).empty()) {
        return OrderResult::UnknownSku;
    }
    if (!findShortages(merged, stock).empty()) {
        return OrderResult::InsufficientStock;
    }

    // Every line fits, so reserve all of them together.
    for (const OrderLine& line : merged) {
        if (!stock.reserveStock(line.sku, line.quantity)) {
            return OrderResult::InsufficientStock;
        }
    }
    newId = nextId_++;
    orders_.push_back(std::make_unique<Order>(newId, customerId, merged));
    return OrderResult::Ok;
}

std::vector<std::string> OrderBook::findShortages(const std::vector<OrderLine>& lines,
                                                  const StockMap& stock) const
{
    std::vector<std::string> shortages;
    for (const OrderLine& line : mergeDuplicateLines(lines)) {
        if (line.quantity > stock.quantity(line.sku)) {
            shortages.push_back(line.sku);
        }
    }
    return shortages;
}

std::vector<std::string> OrderBook::findUnknownSkus(const std::vector<OrderLine>& lines,
                                                    const StockMap& stock) const
{
    std::vector<std::string> unknown;
    for (const OrderLine& line : mergeDuplicateLines(lines)) {
        if (!stock.contains(line.sku)) {
            unknown.push_back(line.sku);
        }
    }
    return unknown;
}

const Order* OrderBook::findOrder(int id) const
{
    for (const std::unique_ptr<Order>& order : orders_) {
        if (order->id() == id) {
            return order.get();
        }
    }
    return nullptr;
}

FulfillmentResult OrderBook::fulfillOrder(int id)
{
    for (const std::unique_ptr<Order>& order : orders_) {
        if (order->id() == id) {
            return order->markFulfilled() ? FulfillmentResult::Fulfilled
                                          : FulfillmentResult::AlreadyFulfilled;
        }
    }

    return FulfillmentResult::OrderNotFound;
}

std::size_t OrderBook::size() const
{
    return orders_.size();
}

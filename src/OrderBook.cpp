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
        stock[line.sku] -= line.quantity;
    }
    newId = nextId_++;
    orders_.emplace_back(newId, customerId, merged);
    return OrderResult::Ok;
}

std::vector<std::string> OrderBook::findShortages(const std::vector<OrderLine>& lines,
                                                  const StockMap& stock) const
{
    std::vector<std::string> shortages;
    for (const OrderLine& line : mergeDuplicateLines(lines)) {
        auto it = stock.find(line.sku);
        int available = (it == stock.end()) ? 0 : it->second;
        if (line.quantity > available) {
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
        if (stock.find(line.sku) == stock.end()) {
            unknown.push_back(line.sku);
        }
    }
    return unknown;
}

const Order* OrderBook::findOrder(int id) const
{
    for (const Order& order : orders_) {
        if (order.id() == id) {
            return &order;
        }
    }
    return nullptr;
}

std::size_t OrderBook::size() const
{
    return orders_.size();
}

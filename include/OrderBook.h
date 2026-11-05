#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Order.h"

enum class OrderResult {
    Ok,
    MissingCustomer,
    EmptyOrder,
    InvalidQuantity,
    UnknownSku,
    InsufficientStock
};

enum class FulfillmentResult {
    Fulfilled,
    OrderNotFound,
    AlreadyFulfilled
};

std::string toString(OrderResult result);

class OrderBook {
public:
    // Available quantity per SKU. Stands in for the Inventory class until it is implemented.
    using StockMap = std::unordered_map<std::string, int>;

    // Validates the order and, if every line can be filled, reserves the stock and stores the order.
    // Any failure rejects the whole order and leaves `stock` unchanged.
    OrderResult createOrder(const std::string& customerId, const std::vector<OrderLine>& lines,
                            StockMap& stock, int& newId);

    // SKUs (after merging duplicates) whose requested quantity exceeds available stock.
    std::vector<std::string> findShortages(const std::vector<OrderLine>& lines,
                                           const StockMap& stock) const;

    // SKUs (after merging duplicates) that do not exist in `stock`.
    std::vector<std::string> findUnknownSkus(const std::vector<OrderLine>& lines,
                                             const StockMap& stock) const;

    const Order* findOrder(int id) const;
    FulfillmentResult fulfillOrder(int id);
    std::size_t size() const;

private:
    std::vector<std::unique_ptr<Order>> orders_;
    int nextId_ = 1;
};

#endif

#ifndef ORDERCOMMAND_H
#define ORDERCOMMAND_H

#include <string>
#include <vector>
#include "Order.h"
#include "OrderBook.h"

struct ParsedOrder {
    std::string customerId;
    std::vector<OrderLine> lines;
};

// Parses "order <customerId> <SKU>:<qty> ...". Returns false and sets `error` on bad input.
bool parseOrderCommand(const std::string& input, ParsedOrder& out, std::string& error);

// Parses one console line, runs it against the book and stock, and returns the message to show.
// Handles "order ..." lines. Any other input returns an "unknown command" message.
std::string runOrderCommand(OrderBook& book, OrderBook::StockMap& stock, const std::string& input);

#endif

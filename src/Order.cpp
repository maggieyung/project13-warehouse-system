#include "Order.h"

#include <utility>

Order::Order(int id, std::string customerId, std::vector<OrderLine> lines)
    : id_(id), customerId_(std::move(customerId)), lines_(std::move(lines))
{
}

int Order::id() const
{
    return id_;
}

const std::string& Order::customerId() const
{
    return customerId_;
}

const std::vector<OrderLine>& Order::lines() const
{
    return lines_;
}

std::vector<OrderLine> mergeDuplicateLines(const std::vector<OrderLine>& lines)
{
    std::vector<OrderLine> merged;
    for (const OrderLine& line : lines) {
        bool found = false;
        for (OrderLine& existing : merged) {
            if (existing.sku == line.sku) {
                existing.quantity += line.quantity;
                found = true;
                break;
            }
        }
        if (!found) {
            merged.push_back(line);
        }
    }
    return merged;
}

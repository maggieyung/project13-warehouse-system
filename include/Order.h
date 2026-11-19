#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>

struct OrderLine {
    std::string sku;
    int quantity;
};

enum class OrderStatus {
    Pending,
    Fulfilled
};

class Order {
public:
    Order(int id, std::string customerId, std::vector<OrderLine> lines);

    int id() const;
    const std::string& customerId() const;
    const std::vector<OrderLine>& lines() const;
    OrderStatus status() const;
    bool markFulfilled();

private:
    int id_;
    std::string customerId_;
    std::vector<OrderLine> lines_;
    OrderStatus status_ = OrderStatus::Pending;
};

// Combines lines with the same SKU into one line, keeping first-seen order.
std::vector<OrderLine> mergeDuplicateLines(const std::vector<OrderLine>& lines);

#endif

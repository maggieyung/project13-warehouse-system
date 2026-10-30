#include "OrderCommand.h"

#include <cctype>
#include <sstream>

namespace {

const char* const kUsage = "order <customerId> <SKU>:<qty> [<SKU>:<qty> ...]";

std::vector<std::string> splitWhitespace(const std::string& input)
{
    std::istringstream stream(input);
    std::vector<std::string> tokens;
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Accepts only a whole number greater than 0 with no sign or trailing characters.
bool parseQuantity(const std::string& text, int& quantity)
{
    if (text.empty() || text.size() > 9) {
        return false;
    }
    for (char c : text) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    quantity = std::stoi(text);
    return quantity > 0;
}

std::string joinSkus(const std::vector<std::string>& skus)
{
    std::string joined;
    for (const std::string& sku : skus) {
        if (!joined.empty()) {
            joined += ", ";
        }
        joined += sku;
    }
    return joined;
}

} // namespace

bool parseOrderCommand(const std::string& input, ParsedOrder& out, std::string& error)
{
    std::vector<std::string> tokens = splitWhitespace(input);
    if (tokens.empty() || tokens[0] != "order") {
        error = std::string("command must start with 'order'. Use: ") + kUsage;
        return false;
    }
    if (tokens.size() < 2) {
        error = std::string("missing customer id. Use: ") + kUsage;
        return false;
    }
    if (tokens.size() < 3) {
        error = std::string("order needs at least one <SKU>:<qty> line. Use: ") + kUsage;
        return false;
    }

    ParsedOrder parsed;
    parsed.customerId = tokens[1];
    for (std::size_t i = 2; i < tokens.size(); ++i) {
        const std::string& token = tokens[i];
        std::size_t colon = token.rfind(':');
        if (colon == std::string::npos) {
            error = "'" + token + "' is not in <SKU>:<qty> form";
            return false;
        }
        std::string sku = token.substr(0, colon);
        if (sku.empty()) {
            error = "'" + token + "' is missing a SKU";
            return false;
        }
        int quantity = 0;
        if (!parseQuantity(token.substr(colon + 1), quantity)) {
            error = "'" + token + "' needs a whole-number quantity greater than 0";
            return false;
        }
        parsed.lines.push_back({sku, quantity});
    }

    out = parsed;
    return true;
}

std::string runOrderCommand(OrderBook& book, OrderBook::StockMap& stock, const std::string& input)
{
    std::vector<std::string> tokens = splitWhitespace(input);
    if (tokens.empty() || tokens[0] != "order") {
        return std::string("Unknown command. Use: ") + kUsage;
    }

    ParsedOrder parsed;
    std::string error;
    if (!parseOrderCommand(input, parsed, error)) {
        return "Error: " + error;
    }

    int newId = 0;
    OrderResult result = book.createOrder(parsed.customerId, parsed.lines, stock, newId);
    switch (result) {
    case OrderResult::Ok: {
        std::string message = "Order " + std::to_string(newId) + " created for " +
                              parsed.customerId + ". Reserved: ";
        const Order* order = book.findOrder(newId);
        for (std::size_t i = 0; i < order->lines().size(); ++i) {
            const OrderLine& line = order->lines()[i];
            if (i > 0) {
                message += ", ";
            }
            message += line.sku + " x" + std::to_string(line.quantity);
        }
        return message;
    }
    case OrderResult::InsufficientStock:
        return "Rejected: insufficient stock for " +
               joinSkus(book.findShortages(parsed.lines, stock)) + ". No stock was reserved.";
    case OrderResult::UnknownSku:
        return "Rejected: unknown SKU " + joinSkus(book.findUnknownSkus(parsed.lines, stock)) +
               ". No stock was reserved.";
    default:
        return "Rejected: " + toString(result);
    }
}

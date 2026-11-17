#include <iostream>
#include <string>
#include "OrderBook.h"
#include "OrderCommand.h"

int main()
{
    OrderBook book;
    OrderBook::StockMap stock;
    stock.addStock("SKU-100", 10);
    stock.addStock("SKU-200", 5);

    std::cout << "Warehouse Order Fulfillment\n"
              << "Sample stock: SKU-100 x10, SKU-200 x5\n"
              << "Commands: order <customerId> <SKU>:<qty> [<SKU>:<qty> ...] | "
              << "fulfill <orderId> | quit\n";

    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        if (line == "quit") {
            break;
        }
        if (line.find_first_not_of(" \t") == std::string::npos) {
            continue;
        }
        std::cout << runOrderCommand(book, stock, line) << '\n';
    }
    return 0;
}

#include "product.h"
#include <iostream>
#include <string>

int main()
{
    std::string sku;
    std::string name;

    std::cout << "Enter SKU: ";
    std::getline(std::cin, sku);

    std::cout << "Enter product name: ";
    std::getline(std::cin, name);

    if (!isValidProduct(sku, name)) {
        std::cout << "Error: SKU and name are required.\n";
        return 1;
    }

    Product product(sku, name);

    std::cout << "\nProduct created:\n";
    std::cout << "SKU: " << product.getSku() << "\n";
    std::cout << "Name: " << product.getName() << "\n";

    return 0;
}

#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
private:
    std::string sku;
    std::string name;

public:
    Product(std::string productSku, std::string productName);

    std::string getSku();
    std::string getName();
};

bool isValidProduct(std::string sku, std::string name);

#endif

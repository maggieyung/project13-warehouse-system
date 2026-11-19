#include "product.h"

Product::Product(std::string productSku, std::string productName)
{
    sku = productSku;
    name = productName;
}

std::string Product::getSku()
{
    return sku;
}

std::string Product::getName()
{
    return name;
}

bool isValidProduct(std::string sku, std::string name)
{
    if (sku.empty() || name.empty()) {
        return false;
    }

    return true;
}

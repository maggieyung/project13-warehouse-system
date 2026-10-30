#include "product.h"
#include <stdexcept>

Product::Product(const std::string& sku, const std::string& name)
{
    if (sku.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw std::invalid_argument("SKU cannot be blank.");
    }

    if (name.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw std::invalid_argument("Product name cannot be blank.");
    }

    sku_ = sku;
    name_ = name;
}

const std::string& Product::getSku() const
{
    return sku_;
}

const std::string& Product::getName() const
{
    return name_;
}

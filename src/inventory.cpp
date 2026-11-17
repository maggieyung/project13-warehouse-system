#include "inventory.h"

int Inventory::quantity(const std::string& sku) const
{
    auto it = quantities_.find(sku);

    if (it != quantities_.end()) {
        return it->second;
    }

    return 0;
}

bool Inventory::contains(const std::string& sku) const
{
    return quantities_.find(sku) != quantities_.end();
}

bool Inventory::addStock(const std::string& sku, int amount)
{
    if (sku.empty() || amount <= 0) {
        return false;
    }

    quantities_[sku] += amount;
    return true;
}

bool Inventory::reserveStock(const std::string& sku, int amount)
{
    if (sku.empty() || amount <= 0) {
        return false;
    }

    auto it = quantities_.find(sku);
    if (it == quantities_.end() || it->second < amount) {
        return false;
    }

    it->second -= amount;
    return true;
}
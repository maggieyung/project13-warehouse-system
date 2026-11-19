#include <string>
#include <unordered_map>

class Inventory {
public:
    int quantity(const std::string& sku) const;
    bool contains(const std::string& sku) const;
    bool addStock(const std::string& sku, int amount);
    bool reserveStock(const std::string& sku, int amount);

private:
    std::unordered_map<std::string, int> quantities_;
};

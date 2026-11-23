#ifndef ORDERS_PRODUCTTYPE_H
#define ORDERS_PRODUCTTYPE_H

#include <memory>
#include <string>
#include <unordered_map>

class ProductType {
public:
    static std::shared_ptr<const ProductType> getType(const std::string& category);
    static std::shared_ptr<const ProductType> getType(const std::string& category,
                                                      double weight, double price);

    const std::string category;
    const double baseWeight;
    const double basePrice;

private:
    using Catalog = std::unordered_map<std::string, std::shared_ptr<const ProductType>>;
    static Catalog& catalog();

    ProductType(double weight, const std::string& category, double price);
};

#endif

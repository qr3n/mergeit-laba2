#include "ProductType.h"

#include <cmath>
#include <stdexcept>

ProductType::Catalog& ProductType::catalog() {
    static Catalog types;
    return types;
}

ProductType::ProductType(double weight, const std::string& category, double price)
    : category(category), baseWeight(weight), basePrice(price) {}

std::shared_ptr<const ProductType> ProductType::getType(const std::string& category) {
    return catalog().at(category);
}

std::shared_ptr<const ProductType> ProductType::getType(const std::string& category,
                                                       double weight, double price) {
    if (category.empty() || !std::isfinite(weight) || weight < 0 ||
        !std::isfinite(price) || price < 0) {
        throw std::invalid_argument("Invalid product category");
    }

    auto& type = catalog()[category];
    if (!type) {
        type.reset(new ProductType(weight, category, price));
    } else if (type->baseWeight != weight || type->basePrice != price) {
        throw std::invalid_argument("Conflicting category definition");
    }
    return type;
}

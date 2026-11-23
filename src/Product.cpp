#include "Product.h"

#include <cmath>
#include <limits>
#include <stdexcept>

Product::Product(int id, const std::shared_ptr<const ProductType>& type, int quantity)
    : Product(id, type, quantity, type ? type->basePrice : 0) {}

Product::Product(int id, std::shared_ptr<const ProductType> type, int quantity, double price)
    : id_(id), quantity_(quantity), type_(std::move(type)), currentPrice_(price) {
    if (!type_ || quantity <= 0 || !std::isfinite(price) || price < 0) {
        throw std::invalid_argument("Invalid product");
    }
}

int Product::getId() const noexcept {
    return id_;
}

double Product::getWeight() const noexcept {
    return quantity_ * type_->baseWeight;
}

double Product::getTotalPrice() const noexcept {
    return quantity_ * currentPrice_;
}

void Product::changeQuantity(int delta) {
    const auto quantity = static_cast<long long>(quantity_) + delta;
    if (quantity <= 0 || quantity > std::numeric_limits<int>::max()) {
        throw std::invalid_argument("Invalid quantity");
    }
    quantity_ = static_cast<int>(quantity);
}

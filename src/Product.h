#ifndef ORDERS_PRODUCT_H
#define ORDERS_PRODUCT_H

#include "ProductType.h"

class Product {
public:
    Product(int id, const std::shared_ptr<const ProductType>& type, int quantity);
    Product(int id, std::shared_ptr<const ProductType> type, int quantity, double price);

    [[nodiscard]] int getId() const noexcept;
    [[nodiscard]] double getWeight() const noexcept;
    [[nodiscard]] double getTotalPrice() const noexcept;
    void changeQuantity(int delta);

private:
    int id_;
    int quantity_;
    std::shared_ptr<const ProductType> type_;
    double currentPrice_;
};

#endif

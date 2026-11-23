#ifndef ORDERS_POSTDELIVERY_H
#define ORDERS_POSTDELIVERY_H

#include "Delivery.h"

class PostDelivery final : public Delivery {
public:
    [[nodiscard]] double getCost(const Order& order) const override;
    [[nodiscard]] std::string getEstimatedTime() const override;
    [[nodiscard]] bool isAvailable(const Order& order) const override;
};

#endif

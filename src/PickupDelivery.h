#ifndef ORDERS_PICKUPDELIVERY_H
#define ORDERS_PICKUPDELIVERY_H

#include "Delivery.h"

class PickupDelivery final : public Delivery {
public:
    [[nodiscard]] double getCost(const Order& order) const override;
    [[nodiscard]] std::string getEstimatedTime() const override;
    [[nodiscard]] bool isAvailable(const Order& order) const override;
};

#endif

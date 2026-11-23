#ifndef ORDERS_COURIERDELIVERY_H
#define ORDERS_COURIERDELIVERY_H

#include "Delivery.h"

class CourierDelivery final : public Delivery {
public:
    [[nodiscard]] double getCost(const Order& order) const override;
    [[nodiscard]] std::string getEstimatedTime() const override;
    [[nodiscard]] bool isAvailable(const Order& order) const override;
};

#endif

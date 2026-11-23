#include "CourierDelivery.h"

#include "Order.h"

double CourierDelivery::getCost(const Order&) const {
    return 500;
}

std::string CourierDelivery::getEstimatedTime() const {
    return "1-2 days";
}

bool CourierDelivery::isAvailable(const Order& order) const {
    return order.getTotalWeight() <= 20;
}

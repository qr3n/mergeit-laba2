#include "PostDelivery.h"

#include "Order.h"

double PostDelivery::getCost(const Order&) const {
    return 200;
}

std::string PostDelivery::getEstimatedTime() const {
    return "5-10 days";
}

bool PostDelivery::isAvailable(const Order& order) const {
    return order.getTotalWeight() <= 10;
}

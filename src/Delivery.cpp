#include "Delivery.h"

#include "Order.h"

Order& operator<<(std::shared_ptr<const Delivery> delivery, Order& order) {
    order.setDelivery(std::move(delivery));
    return order;
}

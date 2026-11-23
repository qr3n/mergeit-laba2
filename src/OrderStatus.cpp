#include "OrderStatus.h"

#include <stdexcept>

const char* toString(OrderStatus status) {
    switch (status) {
        case OrderStatus::Pending: return "pending";
        case OrderStatus::Paid: return "paid";
        case OrderStatus::Shipped: return "shipped";
        case OrderStatus::Delivered: return "delivered";
        case OrderStatus::Cancelled: return "cancelled";
    }
    throw std::invalid_argument("Unknown status");
}

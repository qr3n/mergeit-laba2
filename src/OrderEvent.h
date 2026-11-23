#ifndef ORDERS_ORDEREVENT_H
#define ORDERS_ORDEREVENT_H

#include "OrderStatus.h"

#include <chrono>

struct OrderEvent {
    int orderId = 0;
    OrderStatus oldStatus = OrderStatus::Pending;
    OrderStatus newStatus = OrderStatus::Pending;
    std::chrono::system_clock::time_point timestamp;
};

#endif

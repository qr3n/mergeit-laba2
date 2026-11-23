#ifndef ORDERS_ORDERSTATUS_H
#define ORDERS_ORDERSTATUS_H

#include <cstdint>

enum class OrderStatus : std::uint8_t { Pending, Paid, Shipped, Delivered, Cancelled };

const char* toString(OrderStatus status);

#endif

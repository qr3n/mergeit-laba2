#include "PercentDiscount.h"

#include "Order.h"

void PercentDiscount::apply(Order& order) const {
    order.setPercentDiscount(10);
}

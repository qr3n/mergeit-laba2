#include "CashbackDiscount.h"

#include "Order.h"

void CashbackDiscount::apply(Order& order) const {
    order.setCashbackRate(5);
}

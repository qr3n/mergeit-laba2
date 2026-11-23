#ifndef ORDERS_CASHBACKDISCOUNT_H
#define ORDERS_CASHBACKDISCOUNT_H

#include "Discount.h"

class CashbackDiscount final : public Discount {
public:
    void apply(Order& order) const override;
};

#endif

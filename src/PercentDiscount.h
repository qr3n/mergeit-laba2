#ifndef ORDERS_PERCENTDISCOUNT_H
#define ORDERS_PERCENTDISCOUNT_H

#include "Discount.h"

class PercentDiscount final : public Discount {
public:
    void apply(Order& order) const override;
};

#endif

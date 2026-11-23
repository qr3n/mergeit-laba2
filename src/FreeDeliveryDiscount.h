#ifndef ORDERS_FREEDELIVERYDISCOUNT_H
#define ORDERS_FREEDELIVERYDISCOUNT_H

#include "Discount.h"

class FreeDeliveryDiscount final : public Discount {
public:
    void apply(Order& order) const override;
};

#endif

#ifndef ORDERS_GIFTDISCOUNT_H
#define ORDERS_GIFTDISCOUNT_H

#include "Discount.h"

class GiftDiscount final : public Discount {
public:
    void apply(Order& order) const override;
};

#endif

#ifndef ORDERS_DISCOUNT_H
#define ORDERS_DISCOUNT_H

class Order;

class Discount {
public:
    virtual ~Discount() = default;
    virtual void apply(Order& order) const = 0;

protected:
    Discount() = default;
    Discount(const Discount&) = default;
    Discount(Discount&&) = default;
    Discount& operator=(const Discount&) = default;
    Discount& operator=(Discount&&) = default;
};

#endif

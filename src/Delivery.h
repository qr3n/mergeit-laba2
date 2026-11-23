#ifndef ORDERS_DELIVERY_H
#define ORDERS_DELIVERY_H

#include <memory>
#include <string>

class Order;

class Delivery {
public:
    virtual ~Delivery() = default;
    [[nodiscard]] virtual double getCost(const Order& order) const = 0;
    [[nodiscard]] virtual std::string getEstimatedTime() const = 0;
    [[nodiscard]] virtual bool isAvailable(const Order& order) const = 0;

protected:
    Delivery() = default;
    Delivery(const Delivery&) = default;
    Delivery(Delivery&&) = default;
    Delivery& operator=(const Delivery&) = default;
    Delivery& operator=(Delivery&&) = default;
};

Order& operator<<(std::shared_ptr<const Delivery> delivery, Order& order);

#endif

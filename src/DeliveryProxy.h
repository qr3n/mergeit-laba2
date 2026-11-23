#ifndef ORDERS_DELIVERYPROXY_H
#define ORDERS_DELIVERYPROXY_H

#include "Delivery.h"

#include <map>
#include <set>

class DeliveryProxy final : public Delivery {
public:
    DeliveryProxy(std::shared_ptr<const Delivery> delivery, double maxWeight,
                  std::set<std::string> regions);

    [[nodiscard]] double getCost(const Order& order) const override;
    [[nodiscard]] std::string getEstimatedTime() const override;
    [[nodiscard]] bool isAvailable(const Order& order) const override;

private:
    std::shared_ptr<const Delivery> realDelivery_;
    double maxWeight_;
    std::set<std::string> regions_;
    mutable std::map<std::pair<std::string, double>, double> costs_;
};

#endif

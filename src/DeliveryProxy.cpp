#include "DeliveryProxy.h"

#include "LoggingProxy.h"
#include "Order.h"

#include <stdexcept>

DeliveryProxy::DeliveryProxy(std::shared_ptr<const Delivery> delivery, double maxWeight,
                             std::set<std::string> regions)
    : realDelivery_(std::move(delivery)), maxWeight_(maxWeight), regions_(std::move(regions)) {
    if (!realDelivery_ || !(maxWeight >= 0) || regions_.empty()) {
        throw std::invalid_argument("Invalid delivery configuration");
    }
}

bool DeliveryProxy::isAvailable(const Order& order) const {
    return order.getTotalWeight() <= maxWeight_ && regions_.count(order.getRegion()) &&
           realDelivery_->isAvailable(order);
}

std::string DeliveryProxy::getEstimatedTime() const {
    return realDelivery_->getEstimatedTime();
}

double DeliveryProxy::getCost(const Order& order) const {
    LoggingProxy::log("DELIVERY", order.getId(), "quote requested");
    if (!isAvailable(order)) {
        throw std::domain_error("Delivery unavailable");
    }
    const auto key = std::make_pair(order.getRegion(), order.getTotalWeight());
    const auto cached = costs_.find(key);
    if (cached != costs_.end()) {
        LoggingProxy::log("DELIVERY", order.getId(), "cached quote");
        return cached->second;
    }
    return costs_.emplace(key, realDelivery_->getCost(order)).first->second;
}

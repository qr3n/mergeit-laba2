#include "Order.h"

#include "LoggingProxy.h"

#include <algorithm>
#include <cmath>
#include <ostream>
#include <stdexcept>

Order::Order(int id, const std::string& customer, const std::string& region)
    : id_(id), customer_(customer), region_(region) {
    if (customer.empty() || region.empty()) {
        throw std::invalid_argument("Customer and region are required");
    }
}

int Order::getId() const noexcept {
    return id_;
}

const std::string& Order::getCustomer() const noexcept {
    return customer_;
}

const std::string& Order::getRegion() const noexcept {
    return region_;
}

OrderStatus Order::getStatus() const noexcept {
    return status_;
}

const std::vector<std::shared_ptr<const Product>>& Order::getItems() const noexcept {
    return items_;
}

double Order::getTotalWeight() const {
    double weight = 0;
    for (const auto& item : items_) {
        weight += item->getWeight();
    }
    return weight;
}

double Order::getTotalPrice() const {
    if (paidTotal_) {
        return *paidTotal_;
    }
    double goods = 0;
    for (const auto& item : items_) {
        goods += item->getTotalPrice();
    }
    const double shipping = delivery_ && !discounts_.freeDelivery ? delivery_->getCost(*this) : 0;
    return std::round((goods + shipping) * (100 - discounts_.percent)) / 100;
}

double Order::getCashback() const {
    return discounts_.cashbackRate == 0 ? 0 : std::round(getTotalPrice() * discounts_.cashbackRate) / 100;
}

void Order::requirePending() const {
    if (status_ != OrderStatus::Pending) {
        throw std::logic_error("Only pending orders can be edited");
    }
}

void Order::addItem(const Product& product) {
    requirePending();
    for (const auto& item : items_) {
        if (item->getId() == product.getId()) {
            throw std::invalid_argument("Duplicate product ID");
        }
    }
    items_.push_back(std::make_shared<const Product>(product));
    try {
        if (delivery_ && !delivery_->isAvailable(*this)) {
            throw std::domain_error("Item exceeds delivery limits");
        }
    } catch (...) {
        items_.pop_back();
        throw;
    }
    LoggingProxy::log("ORDER", id_, "item added");
}

void Order::removeItem(int productId) {
    requirePending();
    const auto item = std::find_if(items_.begin(), items_.end(), [productId](const auto& value) {
        return value->getId() == productId;
    });
    if (item == items_.end()) {
        throw std::out_of_range("Unknown product");
    }
    items_.erase(item);
    LoggingProxy::log("ORDER", id_, "item removed");
}

void Order::setDelivery(std::shared_ptr<const Delivery> delivery) {
    requirePending();
    if (!delivery || !delivery->isAvailable(*this)) {
        throw std::domain_error("Delivery unavailable");
    }
    delivery_ = std::move(delivery);
    LoggingProxy::log("DELIVERY", id_, "assigned");
}

void Order::applyDiscount(const Discount& discount) {
    requirePending();
    discount.apply(*this);
    LoggingProxy::log("DISCOUNT", id_, "applied");
}

void Order::setPercentDiscount(unsigned percent) {
    requirePending();
    if (percent > 100) {
        throw std::invalid_argument("Percentage exceeds 100");
    }
    discounts_.percent = percent;
}

void Order::setCashbackRate(unsigned percent) {
    requirePending();
    if (percent > 100) {
        throw std::invalid_argument("Percentage exceeds 100");
    }
    discounts_.cashbackRate = percent;
}

void Order::waiveDeliveryFee() {
    requirePending();
    discounts_.freeDelivery = true;
}

void Order::setStatus(OrderStatus status) {
    const bool valid =
        (status_ == OrderStatus::Pending && (status == OrderStatus::Paid || status == OrderStatus::Cancelled)) ||
        (status_ == OrderStatus::Paid && status == OrderStatus::Shipped) ||
        (status_ == OrderStatus::Shipped && status == OrderStatus::Delivered);
    if (!valid || (status == OrderStatus::Paid && items_.empty())) {
        throw std::logic_error("Invalid order transition");
    }
    if (status == OrderStatus::Paid) {
        paidTotal_ = getTotalPrice();
    }
    const OrderEvent event{id_, status_, status, std::chrono::system_clock::now()};
    status_ = status;
    LoggingProxy::log("ORDER", id_, toString(status));
    notify(event);
}

void Order::subscribe(IObserver* observer) {
    if (!observer) {
        throw std::invalid_argument("Missing observer");
    }
    if (std::find(observers_.begin(), observers_.end(), observer) == observers_.end()) {
        observers_.push_back(observer);
        LoggingProxy::log("ORDER", id_, "observer subscribed");
    }
}

void Order::unsubscribe(IObserver* observer) {
    const auto found = std::find(observers_.begin(), observers_.end(), observer);
    if (found != observers_.end()) {
        observers_.erase(found);
        LoggingProxy::log("ORDER", id_, "observer unsubscribed");
    }
}

void Order::notify(const OrderEvent& event) {
    const auto subscribers = observers_;
    for (auto* const observer : subscribers) {
        if (std::find(observers_.begin(), observers_.end(), observer) != observers_.end()) {
            observer->onOrderEvent(event);
        }
    }
}

Order& Order::operator+(const Discount& discount) {
    applyDiscount(discount);
    return *this;
}

Order& Order::operator+(const Product& product) {
    addItem(product);
    return *this;
}

Order& Order::operator-(int productId) {
    removeItem(productId);
    return *this;
}

double Order::operator*(unsigned copies) const {
    return getTotalPrice() * copies;
}

std::ostream& operator<<(std::ostream& stream, const Order& order) {
    return stream << '#' << order.getId() << ' ' << order.getCustomer() << ' '
                  << toString(order.getStatus()) << " | " << order.getTotalWeight()
                  << " kg | " << order.getTotalPrice() << " RUB";
}

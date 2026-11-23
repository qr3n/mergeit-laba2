#include "OrderManager.h"

#include "LoggingProxy.h"

#include <limits>
#include <stdexcept>

OrderManager::OrderManager(Limits limits) : limits_(limits) {}

Order& OrderManager::createOrder(const std::string& customer, const std::vector<Product>& items,
                                 const std::string& region) {
    const auto account = customers_.find(customer);
    if (active_ >= limits_.total || limits_.perCustomer == 0 ||
        (account != customers_.end() && account->second.active >= limits_.perCustomer)) {
        throw std::domain_error("Active order limit reached");
    }
    if (orders_.size() >= std::numeric_limits<int>::max()) {
        throw std::overflow_error("Order IDs exhausted");
    }

    const int id = static_cast<int>(orders_.size()) + 1;
    auto& order = orders_.try_emplace(id, id, customer, region).first->second;
    try {
        for (const auto& item : items) {
            order.addItem(item);
        }
        order.subscribe(this);
        ++customers_[customer].active;
    } catch (...) {
        orders_.erase(id);
        throw;
    }
    ++active_;
    LoggingProxy::log("MANAGER", id, "created");
    return order;
}

void OrderManager::updateStatus(int orderId, OrderStatus status) {
    orders_.at(orderId).setStatus(status);
}

void OrderManager::onOrderEvent(const OrderEvent& event) {
    const auto& order = orders_.at(event.orderId);
    auto& account = customers_.at(order.getCustomer());
    if (event.newStatus == OrderStatus::Paid) {
        const double amount = order.getTotalPrice();
        payments_.emplace_back(event.timestamp, amount);
        turnover_ += amount;
        account.bonuses += order.getCashback();
    }
    if (event.newStatus == OrderStatus::Cancelled || event.newStatus == OrderStatus::Delivered) {
        --account.active;
        --active_;
    }
    LoggingProxy::log("MANAGER", event.orderId, toString(event.newStatus));
}

double OrderManager::getTurnover() const noexcept {
    return turnover_;
}

double OrderManager::getTurnover(TimePoint from, TimePoint until) const {
    if (from > until) {
        throw std::invalid_argument("Invalid reporting period");
    }
    double total = 0;
    for (const auto& [time, amount] : payments_) {
        if (from <= time && time < until) {
            total += amount;
        }
    }
    return total;
}

double OrderManager::getBonusBalance(const std::string& customer) const {
    return customers_.at(customer).bonuses;
}

std::size_t OrderManager::getCustomerCount() const noexcept {
    return customers_.size();
}

std::size_t OrderManager::getActiveCount() const noexcept {
    return active_;
}

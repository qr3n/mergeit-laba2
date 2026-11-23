#ifndef ORDERS_ORDERMANAGER_H
#define ORDERS_ORDERMANAGER_H

#include "Order.h"

#include <map>

class OrderManager final : public IObserver {
public:
    using TimePoint = std::chrono::system_clock::time_point;

    struct Limits {
        std::size_t perCustomer = 5;
        std::size_t total = 100;
    };

    OrderManager() = default;
    explicit OrderManager(Limits limits);
    ~OrderManager() override = default;
    OrderManager(OrderManager&&) = delete;
    OrderManager& operator=(OrderManager&&) = delete;
    OrderManager(const OrderManager&) = delete;
    OrderManager& operator=(const OrderManager&) = delete;

    Order& createOrder(const std::string& customer, const std::vector<Product>& items,
                       const std::string& region = "local");
    void updateStatus(int orderId, OrderStatus status);
    [[nodiscard]] double getTurnover() const noexcept;
    [[nodiscard]] double getTurnover(TimePoint from, TimePoint until) const;
    [[nodiscard]] double getBonusBalance(const std::string& customer) const;
    [[nodiscard]] std::size_t getCustomerCount() const noexcept;
    [[nodiscard]] std::size_t getActiveCount() const noexcept;

private:
    struct Account {
        std::size_t active = 0;
        double bonuses = 0;
    };

    void onOrderEvent(const OrderEvent& event) override;

    Limits limits_;
    std::size_t active_ = 0;
    std::map<int, Order> orders_;
    std::map<std::string, Account> customers_;
    std::vector<std::pair<TimePoint, double>> payments_;
    double turnover_ = 0;
};

#endif

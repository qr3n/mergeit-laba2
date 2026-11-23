#ifndef ORDERS_ORDER_H
#define ORDERS_ORDER_H

#include "Delivery.h"
#include "Discount.h"
#include "IObservable.h"
#include "Product.h"

#include <iosfwd>
#include <optional>
#include <vector>

class Order final : public IObservable {
public:
    Order(int id, const std::string& customer, const std::string& region = "local");
    ~Order() override = default;
    Order(Order&&) = delete;
    Order& operator=(Order&&) = delete;
    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete;

    [[nodiscard]] int getId() const noexcept;
    [[nodiscard]] const std::string& getCustomer() const noexcept;
    [[nodiscard]] const std::string& getRegion() const noexcept;
    [[nodiscard]] OrderStatus getStatus() const noexcept;
    [[nodiscard]] const std::vector<std::shared_ptr<const Product>>& getItems() const noexcept;
    [[nodiscard]] double getTotalWeight() const;
    [[nodiscard]] double getTotalPrice() const;
    [[nodiscard]] double getCashback() const;

    void addItem(const Product& product);
    void removeItem(int productId);
    void applyDiscount(const Discount& discount);
    void setDelivery(std::shared_ptr<const Delivery> delivery);
    void setStatus(OrderStatus status);
    void setPercentDiscount(unsigned percent);
    void setCashbackRate(unsigned percent);
    void waiveDeliveryFee();

    void subscribe(IObserver* observer) override;
    void unsubscribe(IObserver* observer) override;

    Order& operator+(const Discount& discount);
    Order& operator+(const Product& product);
    Order& operator-(int productId);
    [[nodiscard]] double operator*(unsigned copies) const;

private:
    void requirePending() const;
    void notify(const OrderEvent& event) override;

    int id_;
    std::string customer_;
    std::string region_;
    OrderStatus status_ = OrderStatus::Pending;
    std::vector<std::shared_ptr<const Product>> items_;
    std::shared_ptr<const Delivery> delivery_;
    std::vector<IObserver*> observers_;
    struct Discounts {
        unsigned percent = 0;
        unsigned cashbackRate = 0;
        bool freeDelivery = false;
    };

    Discounts discounts_;
    std::optional<double> paidTotal_;
};

std::ostream& operator<<(std::ostream& stream, const Order& order);

#endif

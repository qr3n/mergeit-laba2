#include "CashbackDiscount.h"
#include "CourierDelivery.h"
#include "DeliveryProxy.h"
#include "FreeDeliveryDiscount.h"
#include "GiftDiscount.h"
#include "OrderManager.h"
#include "PercentDiscount.h"
#include "PickupDelivery.h"
#include "PostDelivery.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
void check(bool condition, const char* const message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

template<class Exception, class Action>
void expectFailure(Action action) {
    try {
        action();
    } catch (const Exception&) {
        return;
    }
    throw std::runtime_error("Expected operation to fail");
}

class CountingDelivery final : public Delivery {
public:
    mutable int quotes = 0;

    double getCost(const Order& order) const override {
        ++quotes;
        return order.getTotalWeight();
    }

    std::string getEstimatedTime() const override {
        return "today";
    }

    bool isAvailable(const Order& order) const override {
        return order.getTotalWeight() <= 10;
    }
};

class Listener final : public IObserver {
public:
    std::vector<OrderEvent> events;

    void onOrderEvent(const OrderEvent& event) override {
        events.push_back(event);
    }
};

void testShipping() {
    const auto type = ProductType::getType("Parcel", 1, 100);
    check(type == ProductType::getType("Parcel"), "Catalog must share categories");
    Order order(1, "A");
    const auto rate = std::make_shared<CountingDelivery>();
    auto proxy = std::make_shared<DeliveryProxy>(rate, 9, std::set<std::string>{"local"});
    order + Product(1, type, 8);
    proxy << order;
    check(order.getTotalPrice() == 808 && proxy->getCost(order) == 8 && rate->quotes == 1, "Cache hit");
    order + Product(2, type, 1);
    check(proxy->getCost(order) == 9 && rate->quotes == 2, "Weight changed");
    expectFailure<std::domain_error>([&] { order + Product(3, type, 1); });
    check(order.getTotalWeight() == 9, "Rejected item must roll back");
    Order remote(2, "A", "remote");
    expectFailure<std::domain_error>([&] { proxy << remote; });
    order - 2;
    std::make_shared<PickupDelivery>() << order;
    order + Product(2, type, 12);
    check(CourierDelivery{}.isAvailable(order) && !PostDelivery{}.isAvailable(order), "Carrier limits");
    order + Product(3, type, 1);
    check(!CourierDelivery{}.isAvailable(order) && order.getTotalPrice() == 2100, "Pickup capacity");
}

void testCarrierBoundaries() {
    const auto type = ProductType::getType("Parcel", 1, 100);
    Order order(1, "Anna");
    const CourierDelivery courier;
    const PostDelivery post;
    const PickupDelivery pickup;

    order + Product(1, type, 10);
    check(post.isAvailable(order) && post.getCost(order) == 200, "Post accepts 10 kg");
    check(post.getEstimatedTime() == "5-10 days", "Post delivery time");
    order + Product(2, type, 10);
    check(courier.isAvailable(order) && courier.getCost(order) == 500, "Courier accepts 20 kg");
    check(courier.getEstimatedTime() == "1-2 days", "Courier delivery time");
    check(!post.isAvailable(order), "Post rejects overweight order");
    order + Product(3, type, 1);
    check(!courier.isAvailable(order), "Courier rejects overweight order");
    check(pickup.isAvailable(order) && pickup.getCost(order) == 0, "Unlimited pickup");
    check(pickup.getEstimatedTime() == "1-3 days", "Pickup time");
}

void testPromotionsAndPayment() {
    OrderManager manager;
    Product product(0, ProductType::getType("Book", 1, 500), 1);
    auto& order = manager.createOrder("Anna", {product});
    product.changeQuantity(2);
    check(order.getTotalPrice() == 500, "Input product is an independent copy");

    const auto carrier = std::make_shared<CountingDelivery>();
    carrier << order;
    order + PercentDiscount{};
    check(order.getTotalPrice() == 450.90, "Percent includes shipping");
    order + GiftDiscount{};
    check(order.getItems().size() == 2 && order.getItems().back()->getTotalPrice() == 0,
          "Gift has a unique ID and zero price");
    order + CashbackDiscount{};
    check(manager.getBonusBalance("Anna") == 0, "Cashback awaits payment");
    order.setStatus(OrderStatus::Paid);
    const int quotes = carrier->quotes;
    check(order.getTotalPrice() == 451.17 && manager.getTurnover() == 451.17, "Paid total");
    check(manager.getBonusBalance("Anna") == 22.56 && carrier->quotes == quotes, "Frozen payment");
}

void testLifecycle() {
    OrderManager manager(OrderManager::Limits{1, 2});
    const Product product(1, ProductType::getType("Book", 1, 500), 1);
    auto& order = manager.createOrder("Anna", {product});
    expectFailure<std::domain_error>([&] { manager.createOrder("Anna", {}); });
    auto& other = manager.createOrder("Boris", {});
    expectFailure<std::domain_error>([&] { manager.createOrder("Chris", {}); });
    other.setStatus(OrderStatus::Cancelled);
    manager.createOrder("Boris", {});
    std::make_shared<PostDelivery>() << order;
    check(order.getTotalPrice() == 700, "Post rate");
    order + PercentDiscount{} + GiftDiscount{} + FreeDeliveryDiscount{} + CashbackDiscount{};
    check(order.getTotalPrice() == 450 && order.getItems().size() == 2, "Combined discounts");

    check(order * 3 == 1350, "Quote for three identical orders");

    Listener listener;
    order.subscribe(&listener);
    order.subscribe(&listener);
    manager.updateStatus(order.getId(), OrderStatus::Paid);
    order.unsubscribe(&listener);
    expectFailure<std::logic_error>([&] { order.setStatus(OrderStatus::Paid); });
    expectFailure<std::logic_error>([&] { order - 1; });
    order.setStatus(OrderStatus::Shipped);
    order.setStatus(OrderStatus::Delivered);
    check(listener.events.size() == 1 && listener.events.front().oldStatus == OrderStatus::Pending,
          "Observer subscription");
    const auto paid = listener.events.front().timestamp;
    check(manager.getTurnover(paid, paid + std::chrono::seconds(1)) == 450 &&
          manager.getTurnover(paid, paid) == 0, "Payment period");
    check(manager.getTurnover() == 450 && manager.getBonusBalance("Anna") == 22.5, "Payment accounting");
    check(manager.getCustomerCount() == 2 && manager.getActiveCount() == 1, "Customer statistics");
    std::ostringstream output;
    output << order;
    check(output.str().find("delivered") != std::string::npos, "Order output");
}
}

int main() {
    try {
        testCarrierBoundaries();
        testShipping();
        testPromotionsAndPayment();
        testLifecycle();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}

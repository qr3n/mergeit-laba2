#include "CashbackDiscount.h"
#include "CourierDelivery.h"
#include "DeliveryProxy.h"
#include "FreeDeliveryDiscount.h"
#include "GiftDiscount.h"
#include "OrderManager.h"
#include "PercentDiscount.h"
#include "PostDelivery.h"

#include <iostream>

int main() {
    OrderManager manager(OrderManager::Limits{1});
    const Product book(1, ProductType::getType("Book", 1, 500), 2);
    auto& order = manager.createOrder("Anna", {});
    order + book + PercentDiscount{};
    std::cout << "1. Discounted order: " << order << '\n';

    const auto courier = std::make_shared<DeliveryProxy>(
        std::make_shared<CourierDelivery>(), 20, std::set<std::string>{"local"});
    courier << order;
    std::cout << "2. Courier: " << courier->getEstimatedTime() << " | " << order << '\n';

    for (const auto status : {OrderStatus::Paid, OrderStatus::Shipped, OrderStatus::Delivered}) {
        manager.updateStatus(order.getId(), status);
    }
    std::cout << "3. Delivered: turnover " << manager.getTurnover() << " RUB\n";

    auto& bundle = manager.createOrder("Anna", {book});
    std::make_shared<PostDelivery>() << bundle;
    bundle + GiftDiscount{} + CashbackDiscount{} + PercentDiscount{} + FreeDeliveryDiscount{};
    bundle.setStatus(OrderStatus::Paid);
    std::cout << "4. Promotions: " << bundle << " | bonuses " << manager.getBonusBalance("Anna") << '\n';

    auto& cancelled = manager.createOrder("Boris", {book});
    cancelled.setStatus(OrderStatus::Cancelled);
    const auto& replacement = manager.createOrder("Boris", {book});
    std::cout << "5. Replacement: " << replacement << " | customers " << manager.getCustomerCount()
              << " | active orders " << manager.getActiveCount() << '\n';
}

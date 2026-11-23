#include "GiftDiscount.h"

#include "Order.h"

#include <limits>
#include <set>
#include <stdexcept>

void GiftDiscount::apply(Order& order) const {
    std::set<int> productIds;
    for (const auto& item : order.getItems()) {
        productIds.insert(item->getId());
    }
    int giftId = 0;
    while (productIds.count(giftId) != 0) {
        if (giftId == std::numeric_limits<int>::max()) {
            throw std::overflow_error("Product IDs exhausted");
        }
        ++giftId;
    }
    order.addItem(Product(giftId, ProductType::getType("Gift mug", 0.3, 250), 1, 0));
}

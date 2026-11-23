#ifndef ORDERS_IOBSERVER_H
#define ORDERS_IOBSERVER_H

#include "OrderEvent.h"

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onOrderEvent(const OrderEvent& event) = 0;

protected:
    IObserver() = default;
    IObserver(const IObserver&) = default;
    IObserver(IObserver&&) = default;
    IObserver& operator=(const IObserver&) = default;
    IObserver& operator=(IObserver&&) = default;
};

#endif

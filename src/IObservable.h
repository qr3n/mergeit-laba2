#ifndef ORDERS_IOBSERVABLE_H
#define ORDERS_IOBSERVABLE_H

#include "IObserver.h"

class IObservable {
public:
    virtual ~IObservable() = default;
    virtual void subscribe(IObserver* observer) = 0;
    virtual void unsubscribe(IObserver* observer) = 0;

protected:
    IObservable() = default;
    IObservable(const IObservable&) = default;
    IObservable(IObservable&&) = default;
    IObservable& operator=(const IObservable&) = default;
    IObservable& operator=(IObservable&&) = default;

    virtual void notify(const OrderEvent& event) = 0;
};

#endif

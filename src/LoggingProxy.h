#ifndef ORDERS_LOGGINGPROXY_H
#define ORDERS_LOGGINGPROXY_H

#include <string_view>

class LoggingProxy {
public:
    static void log(std::string_view source, int orderId, std::string_view operation);
};

#endif

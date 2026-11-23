#include "LoggingProxy.h"

#include <iostream>

void LoggingProxy::log(std::string_view source, int orderId, std::string_view operation) {
    std::clog << '[' << source << "] #" << orderId << ' ' << operation << '\n';
}

cmake_minimum_required(VERSION 3.16)
project(orders LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

add_library(orders_core
    src/CashbackDiscount.cpp
    src/CourierDelivery.cpp
    src/Delivery.cpp
    src/DeliveryProxy.cpp
    src/FreeDeliveryDiscount.cpp
    src/GiftDiscount.cpp
    src/LoggingProxy.cpp
    src/Order.cpp
    src/OrderManager.cpp
    src/OrderStatus.cpp
    src/PercentDiscount.cpp
    src/PickupDelivery.cpp
    src/PostDelivery.cpp
    src/Product.cpp
    src/ProductType.cpp
)

target_include_directories(orders_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(orders_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(orders src/main.cpp)
target_link_libraries(orders PRIVATE orders_core)

if(BUILD_TESTING)
    add_executable(orders_tests tests/OrderTests.cpp)
    target_link_libraries(orders_tests PRIVATE orders_core)
    add_test(NAME orders_tests COMMAND orders_tests)
endif()

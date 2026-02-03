cmake_minimum_required(VERSION 3.21)
project(orders-plugin VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find dependencies
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

# SDK (installed via CMAKE_PREFIX_PATH)
find_package(MPF REQUIRED)

# Plugin library
add_library(orders-plugin SHARED
    src/orders_plugin.cpp
    src/orders_service.cpp
    src/order_model.cpp
    include/orders_plugin.h
    include/orders_service.h
    include/order_model.h
)

target_include_directories(orders-plugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(orders-plugin PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    MPF::foundation-sdk
)

# QML module embedded in plugin
qt_add_qml_module(orders-plugin
    URI YourCo.Orders
    VERSION 1.0
    QML_FILES
        qml/OrdersPage.qml
        qml/OrderDetailPage.qml
        qml/OrderCard.qml
        qml/CreateOrderDialog.qml
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/YourCo/Orders
    NO_PLUGIN
)

# Output to plugins directory
set_target_properties(orders-plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)

# Install
install(TARGETS orders-plugin
    LIBRARY DESTINATION plugins
    RUNTIME DESTINATION plugins
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/qml/YourCo
    DESTINATION qml
)

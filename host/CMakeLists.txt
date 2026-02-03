cmake_minimum_required(VERSION 3.21)
project(mpf-host VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find dependencies
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

# SDK (header-only, direct path)
set(MPF_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../foundation-sdk")

# Generate version header
configure_file(
    cmake/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/mpf/version.h
    @ONLY
)

# Application
add_executable(mpf-host
    # Main
    src/main.cpp
    src/application.cpp
    
    # Core (moved from SDK)
    src/service_registry.cpp
    src/logger.cpp
    src/plugin_metadata.cpp
    
    # Services
    src/plugin_manager.cpp
    src/plugin_loader.cpp
    src/navigation_service.cpp
    src/settings_service.cpp
    src/theme_service.cpp
    src/menu_service.cpp
    src/qml_context.cpp
    
    # Headers
    include/application.h
    include/service_registry.h
    include/logger.h
    include/plugin_metadata.h
    include/plugin_manager.h
    include/plugin_loader.h
    include/navigation_service.h
    include/settings_service.h
    include/theme_service.h
    include/menu_service.h
    include/qml_context.h
)

target_include_directories(mpf-host PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${MPF_SDK_DIR}/include
)

target_link_libraries(mpf-host PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

# QML module
qt_add_qml_module(mpf-host
    URI MPF.Host
    VERSION 1.0
    RESOURCE_PREFIX /
    QML_FILES
        qml/Main.qml
        qml/SideMenu.qml
        qml/MenuItemCustom.qml
        qml/ErrorDialog.qml
    RESOURCES
        qml/images/logo.svg
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/MPF/Host
)

# Output directories  
set_target_properties(mpf-host PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Create plugins directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qml)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/config)

# Copy default config files to build output
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/ DESTINATION ${CMAKE_BINARY_DIR}/config)

# Install
install(TARGETS mpf-host
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/qml/
    DESTINATION qml
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/config/
    DESTINATION config
)

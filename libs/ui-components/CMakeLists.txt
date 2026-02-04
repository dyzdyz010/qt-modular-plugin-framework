cmake_minimum_required(VERSION 3.21)
project(mpf-ui-components VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

# Suppress Qt 6 policy warnings
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

# QML 文件
set(QML_FILES
    qml/MPFCard.qml
    qml/MPFButton.qml
    qml/MPFIconButton.qml
    qml/StatusBadge.qml
    qml/MPFDialog.qml
    qml/MPFTextField.qml
    qml/MPFLoadingIndicator.qml
)

# 使用 qt_add_qml_module 创建带 C++ 后端的 QML 模块
# 注意：QML_ELEMENT 类型的头文件需要能被直接 include
qt_add_qml_module(mpf-ui-components
    URI MPF.Components
    VERSION 1.0
    SOURCES
        src/ui_components_global.h
        src/color_helper.h
        src/color_helper.cpp
        src/input_validator.h
        src/input_validator.cpp
    QML_FILES ${QML_FILES}
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/MPF/Components
    IMPORT_PATH ${CMAKE_BINARY_DIR}/qml
)

add_library(MPF::ui-components ALIAS mpf-ui-components)

# 添加 src 目录到 include 路径，让生成的代码能找到头文件
target_include_directories(mpf-ui-components
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include/mpf/ui>
)

target_link_libraries(mpf-ui-components
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
)

target_compile_definitions(mpf-ui-components PRIVATE MPF_UI_COMPONENTS_EXPORTS)

# Install C++ library and headers
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS mpf-ui-components
    EXPORT MPFUIComponentsTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 安装头文件到 include/mpf/ui/
install(FILES
    src/ui_components_global.h
    src/color_helper.h
    src/input_validator.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mpf/ui
)

# Install QML module
install(DIRECTORY ${CMAKE_BINARY_DIR}/qml/MPF
    DESTINATION qml
)

# CMake package config
install(EXPORT MPFUIComponentsTargets
    FILE MPFUIComponentsTargets.cmake
    NAMESPACE MPF::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MPFUIComponents
)

configure_package_config_file(
    cmake/MPFUIComponentsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MPFUIComponentsConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MPFUIComponents
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MPFUIComponentsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MPFUIComponentsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MPFUIComponentsConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MPFUIComponents
)

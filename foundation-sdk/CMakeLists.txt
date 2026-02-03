cmake_minimum_required(VERSION 3.21)
project(mpf-foundation-sdk VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

# Library
add_library(mpf-foundation-sdk SHARED
    src/service_registry.cpp
    src/logger.cpp
    src/plugin_metadata.cpp
    include/mpf/service_registry.h
    include/mpf/logger.h
    include/mpf/plugin_metadata.h
    include/mpf/interfaces/iplugin.h
    include/mpf/interfaces/ilogger.h
    include/mpf/interfaces/inavigation.h
    include/mpf/interfaces/isettings.h
    include/mpf/interfaces/itheme.h
    include/mpf/interfaces/imenu.h
)

target_include_directories(mpf-foundation-sdk
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(mpf-foundation-sdk
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
)

# Generate version header
configure_file(
    cmake/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/mpf/version.h
    @ONLY
)

# Export symbols
include(GenerateExportHeader)
generate_export_header(mpf-foundation-sdk
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/mpf/mpf_export.h
)
target_include_directories(mpf-foundation-sdk
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

set_target_properties(mpf-foundation-sdk PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    EXPORT_NAME foundation-sdk
)

# Install
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS mpf-foundation-sdk
    EXPORT MPFTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/mpf
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/include/mpf/mpf_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/include/mpf/version.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mpf
)

# CMake config files
configure_package_config_file(
    cmake/MPFConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MPFConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MPF
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MPFConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(EXPORT MPFTargets
    FILE MPFTargets.cmake
    NAMESPACE MPF::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MPF
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MPFConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MPFConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MPF
)

cmake_minimum_required(VERSION 3.21)
project(mpf-http-client VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_library(mpf-http-client SHARED
    src/http_client.cpp
    include/mpf/http/http_client.h
)

add_library(MPF::http-client ALIAS mpf-http-client)

target_include_directories(mpf-http-client
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(mpf-http-client
    PUBLIC
        Qt6::Core
        Qt6::Network
)

target_compile_definitions(mpf-http-client PRIVATE MPF_HTTP_CLIENT_EXPORTS)

# Install + package config
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS mpf-http-client
    EXPORT MPFHttpClientTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/mpf
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT MPFHttpClientTargets
    FILE MPFHttpClientTargets.cmake
    NAMESPACE MPF::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MPFHttpClient
)

configure_package_config_file(
    cmake/MPFHttpClientConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MPFHttpClientConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MPFHttpClient
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MPFHttpClientConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MPFHttpClientConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MPFHttpClientConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MPFHttpClient
)

cmake_minimum_required(VERSION 3.21)
project(mpf-foundation-sdk VERSION 1.0.0 LANGUAGES CXX)

# Header-only interface library
add_library(mpf-foundation-sdk INTERFACE)
add_library(MPF::foundation-sdk ALIAS mpf-foundation-sdk)

target_include_directories(mpf-foundation-sdk
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Require C++17
target_compile_features(mpf-foundation-sdk INTERFACE cxx_std_17)

# Install headers
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(DIRECTORY include/mpf
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
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

install(TARGETS mpf-foundation-sdk
    EXPORT MPFTargets
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

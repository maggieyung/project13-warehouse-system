cmake_minimum_required(VERSION 3.16)
project(WarehouseOrderFulfillment CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
    add_compile_options(-Wall -Wextra)
endif()

# Core domain + command logic, shared by the app and the tests
add_library(warehouse_core
    src/Order.cpp
    src/OrderBook.cpp
    src/OrderCommand.cpp
    src/inventory.cpp
)
target_include_directories(warehouse_core PUBLIC include)

# Console application
add_executable(warehouse src/main.cpp)
target_link_libraries(warehouse PRIVATE warehouse_core)

# Tests (GoogleTest pinned to a release tag so CI does not change under us)
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(warehouse_tests
    test/order_book_test.cpp
    test/order_command_test.cpp
    test/inventory_test.cpp
)
target_link_libraries(warehouse_tests PRIVATE warehouse_core GTest::gtest_main)
gtest_discover_tests(warehouse_tests)

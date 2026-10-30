# Project 13: Warehouse Order Fulfillment System

Capstone project for CS410 Introduction to Software Engineering.


Contributors: Maggie Y. ([@maggieyung](https://github.com/maggieyung)), Kenzo M. ([@Yoken0](https://github.com/Yoken0)), Anni Z. ([@Platurnip](https://github.com/Platurnip)), Vincent L. ([@Wingcent](https://github.com/Wingcent)), Allan M. ([@AllanM328](https://github.com/AllanM328))

## Goal 
Develop a system that manages products, available inventory, customer orders, and order fulfillment.

## Current scope

A C++17 console app. The first working feature (the Milestone 3 vertical slice) is **creating a warehouse order**: an employee types an order, the system checks stock, reserves it, and prints the result. If any line is short, the whole order is rejected and no stock changes. See [docs/milestone3/vertical-slice.md](docs/milestone3/vertical-slice.md).

## Prerequisites

- A C++17 compiler (g++ or clang++)
- CMake 3.16 or newer
- Internet access the first time you configure (CMake downloads GoogleTest)

## Build

```sh
cmake -S . -B build && cmake --build build
```

## Run the app

```sh
./build/warehouse
```

Command format: `order <customerId> <SKU>:<qty> [<SKU>:<qty> ...]`, or `quit` to exit.

Example session (sample stock is `SKU-100` x10 and `SKU-200` x5):

```
> order C1 SKU-100:3
Order 1 created for C1. Reserved: SKU-100 x3
> order C2 SKU-200:99
Rejected: insufficient stock for SKU-200. No stock was reserved.
> order C3 SKU-100:x
Error: 'SKU-100:x' needs a whole-number quantity greater than 0
> quit
```

## Run the tests

```sh
ctest --test-dir build --output-on-failure
```

Tests use GoogleTest (v1.14.0, fetched by CMake) and live in `test/`.

## CI

`.github/workflows/ci.yml` runs on every push and pull request: configure with CMake, build, and run all tests with CTest. It fails if the build fails, any test fails, or no tests are found. Results are in the repo's **Actions** tab and on each PR's checks.

## Current scope and known gaps

- In-memory only: orders and stock are lost when the app exits (no persistence yet).
- Stock is hard-coded sample data in `src/main.cpp`; the `Inventory` class (`include/inventory.h`) is not implemented or wired in yet, so it is not part of the build.
- No Product or Customer classes yet: any SKU in the sample stock and any customer id are accepted.
- Order status, cancelling, shipping, and reports are not implemented yet.

## Project layout

```
Warehouse-system/
├── CMakeLists.txt               build for the app, core library, and tests
├── .github/workflows/ci.yml     CI: configure, build, test
├── docs/milestone3/             vertical slice description
├── include/
│   ├── Order.h                  OrderLine, Order, mergeDuplicateLines
│   ├── OrderBook.h              createOrder, findShortages, OrderResult
│   ├── OrderCommand.h           console command parsing and running
│   └── inventory.h              Inventory (in progress, not built yet)
├── src/
│   ├── Order.cpp / OrderBook.cpp / OrderCommand.cpp
│   ├── inventory.cpp            Inventory (in progress, not built yet)
│   └── main.cpp                 console loop
└── test/
    ├── order_book_test.cpp      domain rules
    ├── order_command_test.cpp   vertical slice (parse → create → message)
    └── inventory_test.cpp       Inventory (in progress, not built yet)
```

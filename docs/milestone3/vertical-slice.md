# Milestone 3 Vertical Slice: Create a Warehouse Order

Backlog item: #2 "Create orders"

## User story

As a warehouse employee, I want to create an order so that customer purchases can be fulfilled.

## Acceptance criteria

| # | Criterion | Tests |
|---|---|---|
| AC1 | A valid order with enough stock succeeds, gets an order id, and reduces available stock by the ordered quantity. | `OrderCommandTest.ValidOrderReservesStock`, `OrderBookTest.ValidOrderIsStoredAndReservesStock`, `OrderBookTest.OrderingExactlyAvailableStockSucceeds` |
| AC2 | If any line is short, the whole order is rejected, the message names the short SKU(s), and no stock changes. | `OrderCommandTest.InsufficientStockRejectsWholeOrder`, `OrderBookTest.OneShortLineRejectsWholeOrderAndKeepsStock`, `OrderBookTest.DuplicateSkusAreCombinedBeforeStockCheck` |
| AC3 | An unknown SKU is rejected with a clear message. | `OrderCommandTest.UnknownSkuIsRejected`, `OrderBookTest.UnknownSkuIsRejected` |
| AC4 | Malformed input (quantity `x` or `0`, no lines, and so on) is rejected with a message, and nothing is created. | `OrderCommandTest.ParsesValidCommand`, `OrderCommandTest.RejectsMalformedQuantity`, `OrderCommandTest.RejectsMissingPartsAndCreatesNothing`, `OrderBookTest.InvalidInputIsRejected` |

## Flow

```
User types a command in the console
  → main.cpp reads the line
  → runOrderCommand() parses the line            (input layer)
  → OrderBook::createOrder() checks and reserves (domain / application logic)
  → findShortages() if rejected for stock        (domain)
  → message string printed to the user           (output)
```

## Business rules

- Lines with the same SKU are combined before the stock check (`SKU-100:6 SKU-100:6` asks for 12).
- Stock is only reserved when every line fits; otherwise nothing changes and no order id is used.

## Out of scope for this slice

- Persistence (everything is in memory).
- The `Inventory`, Product, and Customer classes; stock is sample data in `main.cpp`.
- Order status, cancelling, shipping, editing, and reports.

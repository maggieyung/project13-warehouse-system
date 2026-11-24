# Interactive Demo

`index.html` is a browser version of the warehouse console app. It lets someone try the order system without installing a compiler or building anything. It runs the same command logic as `src/OrderCommand.cpp` and `src/OrderBook.cpp`, ported to JavaScript, and prints the same messages word for word.

## Open it

The page is a single file with no build step. Open it in any modern browser:

```sh
open demo/index.html        # macOS
xdg-open demo/index.html    # Linux
start demo/index.html       # Windows
```

You can also drag the file into a browser window. The page loads its fonts from Google Fonts. If you're offline, it falls back to system fonts and everything else still works.

## What's on the page

| Area | What it does |
|---|---|
| **Console** | Type commands as you would in `./build/warehouse` and press Enter. Replies are colored by outcome: green for created, amber for rejected, red for input errors, blue for fulfilled. |
| **Try a scenario** | Each button runs one command in the console, so you can show a case in one click. |
| **Stock** | Shows how much of each SKU is available and how much is reserved by orders. A row highlights briefly when its stock changes. |
| **Orders** | Lists every order with its customer, lines and status (Pending or Fulfilled). **Fulfill** runs `fulfill <id>` in the console for you. |
| **Reset session** | Clears all orders and puts stock back to the starting sample (SKU-100 x10, SKU-200 x5). |

The page opens with the README's example session already run (one order that succeeds, one that's rejected), so there is something to look at right away.

## Commands

```
order <customerId> <SKU>:<qty> [<SKU>:<qty> ...]
fulfill <orderId>
quit
```

- Quantities must be whole numbers greater than 0, with no sign.
- Order ids must be positive whole numbers.
- `quit` ends the session the same way the app does, and the console stops taking input. Click **Reset session** or any scenario button to start again.
- Blank lines are ignored.
- Press ↑ and ↓ in the console to go through earlier commands.

## A suggested walkthrough

Use this order to show every rule in a couple of minutes. Start with **Reset session** so the stock is fresh.

| Step | Command | What it shows |
|---|---|---|
| 1 | `order C1 SKU-100:3` | A valid order is created and SKU-100 drops from 10 to 7. |
| 2 | `order C2 SKU-100:2 SKU-200:4` | Several lines are reserved together. |
| 3 | `order C3 SKU-200:99` | One short line rejects the whole order, and stock doesn't change. |
| 4 | `order C4 SKU-100:6 SKU-100:6` | Duplicate lines are combined into 12 before the stock check, so this is rejected. |
| 5 | `order C5 SKU-999:1` | Unknown SKUs are rejected by name. |
| 6 | `order C6 SKU-100:x` | Malformed input gets an error, and nothing is created. |
| 7 | `fulfill 1` | Order 1 becomes Fulfilled. Stock doesn't change, because it was reserved when the order was created. |
| 8 | `fulfill 1` | Fulfilling it again is reported as already fulfilled. |
| 9 | `fulfill 42` | An order id that doesn't exist is reported as not found. |

Note that rejected orders don't use up an order id. After step 3, the next successful order still gets the next number in sequence.

## How faithful is it?

The page doesn't connect to the C++ program. It contains its own copy of the logic, between the `// ENGINE START` and `// ENGINE END` markers in the `<script>` block. That copy was checked against the compiled app with 28 inputs, including edge cases such as `fulfill 99999999999`, `fulfill 2x`, `SKU-100:+3` and `A:B:2`, and every reply matched exactly.

Because it's a copy, **it doesn't update itself when the C++ code changes.** When you change a message, add a command, or change a rule in `src/OrderCommand.cpp` or `src/OrderBook.cpp`, make the same change in the engine section of `index.html`. The starting stock and banner text are also copied from `src/main.cpp` (`START_STOCK` and `BANNER`).

## Limitations

The demo has the same gaps as the app itself:

- Nothing is saved. Reloading the page starts over, just as exiting the app does.
- Stock is a fixed sample, loaded into `Inventory` at startup. There's no command to add or adjust stock.
- Any customer id is accepted, because there are no Customer or Product records yet.
- There's no cancelling, shipping, or reporting.

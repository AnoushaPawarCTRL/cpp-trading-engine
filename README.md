# Low-Latency C++ Trading Engine

A high-performance simulated electronic trading engine written in **C++** that models the architecture of modern financial exchanges.
The project demonstrates **concurrency, lock-free data structures, networking, and order book mechanics** used in real-world trading systems.

---

## Overview

Electronic exchanges process millions of orders per second with extremely low latency.
This project simulates a simplified exchange architecture including:

* Multi-threaded order generation
* Lock-free order queues
* Matching engine
* Real-time order book
* TCP trading gateway
* Live market depth visualization

The system is designed to demonstrate core concepts behind **high-frequency trading infrastructure** and **low-latency system design**.

---

## System Architecture

Client / Order Generator
↓
TCP Network Gateway
↓
Lock-Free Order Queue
↓
Matching Engine
↓
Order Book (L1 / L2 / L3)

Orders are produced by multiple threads and placed into a lock-free queue before being processed by the matching engine.

---

## Order Book Model

The engine implements three levels of market data commonly used in financial exchanges:

### Level 1 — Best Bid / Offer (BBO)

Displays the top of book:

* Best bid price
* Best ask price

Example:

Bid: 101.20
Ask: 101.25

---

### Level 2 — Market by Price

Aggregated liquidity at each price level.

Example:

BIDS
101.20 | 500
101.15 | 210

ASKS
101.25 | 300
101.30 | 120

---

### Level 3 — Market by Order

Individual orders are maintained per price level.

Example:

Price: 101.20
OrderID 101 | Qty 200
OrderID 102 | Qty 300

This engine internally maintains **Level 3 data**, from which L2 and L1 views are derived.

---

## Key Components

### Matching Engine

Processes incoming orders and matches buy and sell orders using price-time priority.

### Lock-Free Queue

Used to pass orders from producer threads to the matching engine with minimal contention.

### Multi-threaded Order Simulation

Multiple threads simulate market participants generating orders.

### TCP Trading Gateway

Orders can be received through a socket interface, similar to real exchange gateways.

### Live Market Depth Display

Console visualization of the evolving order book showing bid and ask liquidity.

---

## Technologies Used

* C++
* Multithreading (`std::thread`)
* Lock-free data structures
* TCP sockets (WinSock)
* Concurrent programming techniques

---

## Example Output

------ MARKET DEPTH ------

ASKS
101.25 | 300
101.30 | 120

BIDS
101.20 | 500
101.15 | 210

---

## Build Instructions

Compile using g++:

g++ -Iinclude src/main.cpp src/OrderBook.cpp src/NetworkServer.cpp -lws2_32 -o build/trading_engine

---

## Running the Engine

Start the trading engine:

./build/trading_engine

The engine will simulate live market activity and display real-time market depth.

---

## Learning Goals

This project was built to explore:

* Low-latency systems design
* Exchange matching engines
* Concurrent programming
* Order book mechanics
* Financial market infrastructure

---

## Future Improvements

Potential extensions:

* FIX protocol order gateway
* WebSocket market data feed
* Persistent order logging
* Latency benchmarking
* GUI market visualization

---

## Author

Systems project focused on **high-performance financial infrastructure and trading system design**.

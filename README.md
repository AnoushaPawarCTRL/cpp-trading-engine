# C++ Trading Engine (Low-Latency + Distributed System)

## Overview

This project started as a simple C++ order matching engine and gradually evolved into a more realistic **trading system simulation**.

It now includes:

* A multi-threaded C++ matching engine
* A live order book (L1 / L2 / L3)
* A Python signal generator
* A Node.js API layer
* Real-time communication using TCP sockets

The goal was to build something closer to how actual trading systems are structured, rather than just a standalone program.

---

## Architecture

```
Python (signals) → Node.js API → TCP → C++ Engine
```

* Python simulates incoming trade signals
* Node.js acts as a middle layer
* C++ handles execution and order matching

---

## Features

* Multi-threaded order processing

* Lock-free queue for high throughput

* Real-time order matching

* Market depth display:

  * L1: Best bid/ask + spread
  * L2: Aggregated price levels
  * L3: Individual orders

* TCP socket communication between services

---

## Tech Stack

* C++ (core engine)
* Python (signal generation)
* Node.js + Express (API layer)
* TCP sockets (network communication)

---

## How to run

### 1. Compile the engine

```bash
g++ -Iinclude src/main.cpp src/OrderBook.cpp src/NetworkServer.cpp -lws2_32 -o build/trading_engine
```

---

### 2. Start the C++ engine

```bash
./build/trading_engine
```

---

### 3. Start the Node server

```bash
cd node
npm install
node server.js
```

---

### 4. Run the signal generator

```bash
cd python
python signal_generator.py
```

---

## Example output

```
------ MARKET DEPTH ------

BEST BID: 101.10 | BEST ASK: 101.23 | SPREAD: 0.13

ASKS
101.23 | 340
101.45 | 120

BIDS
101.10 | 500
100.95 | 310
```

---

## Why I built this

I wanted to go beyond typical projects and build something that shows:

* understanding of low-latency systems
* how trading engines actually work
* how different components communicate in real systems

---

## Next steps / ideas

* Keep improving latency (persistent connections, better protocols)
* Add a simple frontend to visualise the market
* Replace random signals with ML-based predictions
* Store and analyse data using a database

---

## Notes

This is a simulation, not a real trading system, but the structure is inspired by how real systems are designed.

---

#include <iostream>
#include "OrderBook.h"
#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include "LockFreeQueue.h"
#include "NetworkServer.h"

// Forward declarations
void generateOrders(OrderBook& book, int numOrders, int startId);
void matchingEngine(OrderBook& book);
void displayThread(OrderBook& book);
void runMultiThreadSimulation(OrderBook& book, int totalOrders, int numThreads);

// Global lock-free queue
LockFreeQueue<Order> orderQueue(1000000);

// ---------------- MULTI-THREAD SIM ----------------
void runMultiThreadSimulation(OrderBook& book, int totalOrders, int numThreads)
{
    int ordersPerThread = totalOrders / numThreads;
    std::vector<std::thread> threads;

    // Start matching engine
    std::thread engineThread(matchingEngine, std::ref(book));
    engineThread.detach();

    // Start order generator threads
    for (int i = 0; i < numThreads; i++)
    {
        threads.emplace_back(generateOrders,
                             std::ref(book),
                             ordersPerThread,
                             i * ordersPerThread);
    }

    for (auto& t : threads)
    {
        t.detach(); // run continuously
    }

    std::cout << "Simulation started with "
              << numThreads << " threads...\n";
}

// ---------------- ORDER GENERATOR ----------------
void generateOrders(OrderBook& book, int numOrders, int startId)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> qtyDist(1, 100);
    std::uniform_real_distribution<> priceDist(95.0, 105.0);
    std::uniform_int_distribution<> typeDist(0, 1);

    for (int i = 0; i < numOrders; i++)
    {
        double rawPrice = priceDist(gen);
        double price = std::round(rawPrice * 100.0) / 100.0; // ✅ DEFINE PRICE HERE

        int qty = qtyDist(gen);
        OrderType type = typeDist(gen) == 0 ? OrderType::BUY : OrderType::SELL;

        while (!orderQueue.enqueue(Order(startId + i, type, price, qty)))
        {
            // retry
        }
    }
}

// ---------------- MATCHING ENGINE ----------------
void matchingEngine(OrderBook& book)
{
    Order order;

    static int count = 0;
    if (++count % 100000 == 0)
    {
        std::cout << "Processed: " << count << " orders\n";
    }

    while (true)
    {
        if (orderQueue.dequeue(order))
        {
            book.addOrder(order);
        }
    }
}

// ---------------- DISPLAY ----------------
void displayThread(OrderBook& book)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        system("cls"); // clear console (Windows)
        book.printMarketDepth();
    }
}

// ---------------- MAIN ----------------
int main()
{
    OrderBook book;

    std::cout << "Starting Low-Latency Trading Engine...\n";

    // Start network server (optional but good for demo)
    std::thread networkThread(startServer);
    networkThread.detach();

    // Start display thread
    std::thread display(displayThread, std::ref(book));
    display.detach();

    // Auto-start simulation
    runMultiThreadSimulation(book, 2000000, 4);

    // Keep program alive
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    return 0;
}
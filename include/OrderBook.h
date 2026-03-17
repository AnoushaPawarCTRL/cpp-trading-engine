#pragma once

#include <map>
#include <queue>
#include <functional>
#include "Order.h"
#include <mutex>
#include <atomic>


class OrderBook {

private:

    std::mutex bookMutex;
    std::atomic<long long> ordersProcessed{0};

private:

    std::map<double, std::queue<Order>, std::greater<double>> buyOrders;
    std::map<double, std::queue<Order>> sellOrders;

public:

    void addOrder(const Order& order);
    void matchOrders();
    void printOrderBook();

    long long getProcessedOrders() const
    {
        return ordersProcessed.load();
    }
    void printMarketDepth();

};
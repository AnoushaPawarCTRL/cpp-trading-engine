#include "../include/OrderBook.h"
#include <iostream>

void OrderBook::printMarketDepth()
{
    std::cout << "\n------ MARKET DEPTH ------\n";

    // ================= L1 (BEST BID / ASK) =================
    double bestBid = 0.0;
    double bestAsk = 0.0;

    if (!buyOrders.empty())
        bestBid = buyOrders.begin()->first;

    if (!sellOrders.empty())
        bestAsk = sellOrders.begin()->first;

    if (bestBid > 0 && bestAsk > 0)
    {
        std::cout << "\nBEST BID: " << bestBid
                  << " | BEST ASK: " << bestAsk
                  << " | SPREAD: " << (bestAsk - bestBid)
                  << "\n";
    }

    // ================= L2 (MARKET DEPTH) =================
    std::cout << "\nASKS\n";

    int count = 0;
    for (auto& [price, orders] : sellOrders)
    {
        int totalQty = 0;
        std::queue<Order> q = orders;

        while (!q.empty())
        {
            totalQty += q.front().quantity;
            q.pop();
        }

        std::cout << price << " | " << totalQty << std::endl;

        if (++count == 5) break;
    }

    std::cout << "\nBIDS\n";

    count = 0;
    for (auto& [price, orders] : buyOrders)
    {
        int totalQty = 0;
        std::queue<Order> q = orders;

        while (!q.empty())
        {
            totalQty += q.front().quantity;
            q.pop();
        }

        std::cout << price << " | " << totalQty << std::endl;

        if (++count == 5) break;
    }
}

void OrderBook::addOrder(const Order& order)
{
    if (order.type == OrderType::BUY)
    {
        // Match against lowest sell
        while (!sellOrders.empty())
        {
            auto bestSell = sellOrders.begin();

            if (bestSell->first > order.price)
                break;

            auto& queue = bestSell->second;

            if (!queue.empty())
            {
                queue.pop(); // remove one order (simple match)

                if (queue.empty())
                    sellOrders.erase(bestSell);

                return; // matched → done
            }
        }

        // If no match → add to book
        buyOrders[order.price].push(order);
    }
    else
    {
        // Match against highest buy
        while (!buyOrders.empty())
        {
            auto bestBuy = buyOrders.begin();

            if (bestBuy->first < order.price)
                break;

            auto& queue = bestBuy->second;

            if (!queue.empty())
            {
                queue.pop(); // remove one order

                if (queue.empty())
                    buyOrders.erase(bestBuy);

                return; // matched
            }
        }

        // No match → add to book
        sellOrders[order.price].push(order);
    }
}

void OrderBook::matchOrders()
{

    while (!buyOrders.empty() && !sellOrders.empty())
    {

        auto bestBuy = buyOrders.begin();
        auto bestSell = sellOrders.begin();

        if (bestBuy->first < bestSell->first)
            break;

        Order& buyOrder = bestBuy->second.front();
        Order& sellOrder = bestSell->second.front();

        int tradeQty = std::min(buyOrder.quantity, sellOrder.quantity);

        std::cout << "TRADE EXECUTED: "
                  << tradeQty
                  << " @ "
                  << bestSell->first
                  << std::endl;

        buyOrder.quantity -= tradeQty;
        sellOrder.quantity -= tradeQty;

        if (buyOrder.quantity == 0)
            bestBuy->second.pop();

        if (sellOrder.quantity == 0)
            bestSell->second.pop();

        if (bestBuy->second.empty())
            buyOrders.erase(bestBuy);

        if (bestSell->second.empty())
            sellOrders.erase(bestSell);
    }
}

void OrderBook::printOrderBook()
{
    std::cout << "\n----- ORDER BOOK -----\n";

    std::cout << "\nASKS (SELL):\n";
    for (auto it = sellOrders.rbegin(); it != sellOrders.rend(); ++it)
    {
        std::cout << it->first << " : " << it->second.size() << "\n";
    }

    std::cout << "\nBIDS (BUY):\n";
    for (auto it = buyOrders.begin(); it != buyOrders.end(); ++it)
    {
        std::cout << it->first << " : " << it->second.size() << "\n";
    }

    std::cout << "----------------------\n";
}
#pragma once

enum class OrderType { BUY, SELL };

struct Order {

    int id;
    OrderType type;
    double price;
    int quantity;

    Order() {}

    Order(int id, OrderType type, double price, int quantity)
        : id(id), type(type), price(price), quantity(quantity) {}
};
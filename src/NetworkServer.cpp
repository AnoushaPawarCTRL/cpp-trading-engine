#include "NetworkServer.h"
#include "LockFreeQueue.h"
#include "OrderBook.h"

#include <winsock2.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

extern LockFreeQueue<Order> orderQueue;

void receiveOrderFromAPI(const std::string& signal, int qty, double price)
{
    OrderType type = signal == "BUY" ? OrderType::BUY : OrderType::SELL;
    static int idCounter = 1;
    orderQueue.enqueue(Order(idCounter++, type, price, qty));
}

void startServer()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9000);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "C++ Engine listening on port 9000...\n";

    while (true)
    {
        SOCKET new_socket = accept(server_fd, NULL, NULL);

        char buffer[1024] = {0};
        int valread = recv(new_socket, buffer, 1024, 0);

        std::string msg(buffer);

        std::stringstream ss(msg);
        std::string type;
        int qty;
        double price;

        ss >> type >> qty >> price;

        OrderType orderType = (type == "BUY") ? OrderType::BUY : OrderType::SELL;

        static int id = 1;
        orderQueue.enqueue(Order(id++, orderType, price, qty));

        std::cout << "Received from Node: " << msg << std::endl;

        closesocket(new_socket);
    }

    WSACleanup();
}
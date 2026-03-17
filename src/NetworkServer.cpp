#include "NetworkServer.h"
#include "LockFreeQueue.h"

#include <winsock2.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

extern LockFreeQueue<Order> orderQueue;

void startServer()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    listen(serverSocket, 5);

    std::cout << "Trading Gateway Listening on port 9000\n";

    while(true)
    {
        SOCKET client = accept(serverSocket, nullptr, nullptr);

        char buffer[1024];
        int bytes = recv(client, buffer, sizeof(buffer), 0);

        if(bytes > 0)
        {
            std::stringstream ss(buffer);

            int id, qty;
            double price;
            int type;

            ss >> id >> type >> price >> qty;

            OrderType orderType = (type == 0) ? OrderType::BUY : OrderType::SELL;

            Order order(id, orderType, price, qty);

            while(!orderQueue.enqueue(order)) {}

            std::cout << "Order received from network\n";
        }

        closesocket(client);
    }
}
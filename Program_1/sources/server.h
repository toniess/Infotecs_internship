#pragma once
#include <iostream>


class Server
{
    int socket_;
    int listener;

public:
    Server(int port);

    void send(std::string num);

    void run();
};

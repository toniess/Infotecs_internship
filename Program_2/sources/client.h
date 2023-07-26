#pragma once
#include <iostream>


class Client {
    std::string serverIP_;
    int serverPort_;
    int socket_;
    static const int BUFFER_SIZE = 1024;

public:
    Client(const std::string& serverIP, int serverPort);

    void run(void (*handler) (std::string));

private:
    bool connectToServer();

    void receiveData(void (*handler)(std::string));
};

#include "server.h"

#include <thread>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


Server::Server(int port)
{
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    listen(listener, 1);
}


void Server::send(std::string num)
{
    ::send(socket_, num.c_str(), num.size(), 0);
}


void Server::run()
{
    socket_ = accept(listener, NULL, NULL);
    std::thread reconnectionThread([&](){
        char buffer[1024];
        while(true)
        {
            if (read(socket_, buffer, 1023) <= 0)
            {
                close(socket_);
                socket_ = accept(listener, NULL, NULL);
            }
        }
    });
    reconnectionThread.detach();
}

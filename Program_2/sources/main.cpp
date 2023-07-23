#include <iostream>
#include <thread>

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define PORT 52000

void handleSigPipe(int /*unused*/) {
    std::cout << "ERRORRROOROROR" << std::endl;
}


void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE] = {0};
    while(true)
    {
        int valread = read(clientSocket, buffer, BUFFER_SIZE);
        if (valread > 0) {
            std::string data(buffer);

            if (data.length() >= 2 && atoi(data.c_str()) % 32 == 0) {
                std::cout << "Полученные данные: " << data << std::endl;
            } else {
                std::cout << "Ошибка: Недопустимые данные" << std::endl;
            }
        }
    }
}

int main() {
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(52000); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
    signal(ECONNRESET, handleSigPipe);
    signal(EPIPE, handleSigPipe);

    std::thread t1(handleClient, std::ref(sock));
    t1.join();

//    shutdown(sock, SHUT_WR);
//    close(sock);

    return 0;
}

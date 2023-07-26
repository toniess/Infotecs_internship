#include "client.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


Client::Client(const std::string &serverIP, int serverPort)
    : serverIP_(serverIP), serverPort_(serverPort) {}


void Client::run(void (*handler)(std::string)) {
    while (true) {
        if (!connectToServer()) {
            sleep(1);
            continue;
        }
        receiveData(handler);
        close(socket_);
    }
}


bool Client::connectToServer() {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0) {
        std::cerr << "Ошибка при создании сокета." << std::endl;
        return false;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort_);

    if (inet_pton(AF_INET, serverIP_.c_str(), &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Ошибка в адресе сервера." << std::endl;
        close(socket_);
        return false;
    }

    if (connect(socket_, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        close(socket_);
        return false;
    }
    return true;
}


void Client::receiveData(void (*handler)(std::string)) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        if (read(socket_, buffer, BUFFER_SIZE - 1) <= 0) {
            break;
        }
        std::string data(buffer);
        handler(std::move(data));
    }
}

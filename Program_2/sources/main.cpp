#include <iostream>

#include "client.h"
#include "receivehandler.h"

int main() {
    Client client("127.0.0.1", 52000);
    client.run(&ReceiveHandler::handle);
    return 0;
}

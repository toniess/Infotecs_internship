#include "bufferprocessor.h"

#include <numeric>

#include "safebuffer.h"
#include "server.h"


BufferProcessor::BufferProcessor(SafeBuffer<std::string> &buffer) :
    buffer(buffer) {}


void BufferProcessor::processAndTransfer(Server &server)
{
    while (true) {
        std::string data = buffer.readOnNotified();
        std::cout << data << std::endl;
        int sum = digitSum(std::move(data));
        server.send(std::move(std::to_string(sum)));
    }
}


int BufferProcessor::digitSum(const std::string &s)
{
    return std::accumulate(s.begin(), s.end(), 0, [](const int& sum, const char& c){
        return sum + (::isdigit(c) ? (c - '0') : 0);
    });
}

#pragma once
#include <iostream>
#include "safebuffer.h"
#include "server.h"


class BufferProcessor
{
    SafeBuffer<std::string>& buffer;

public:
    BufferProcessor(SafeBuffer<std::string>& buffer);

    void processAndTransfer(Server& server);

private:
    int digitSum(const std::string& s);
};

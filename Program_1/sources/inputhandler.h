#pragma once
#include <iostream>
#include "safebuffer.h"


class InputHandler
{
    SafeBuffer<std::string>& buffer;

public:
    InputHandler(SafeBuffer<std::string>& buffer);

    void startInputProcess();

private:
    bool isValid(const std::string& s);

    std::string sortAndReplace(std::string s);
};


#include "inputhandler.h"
#include "safebuffer.h"


InputHandler::InputHandler(SafeBuffer<std::string> &buffer) :
    buffer(buffer){}

void InputHandler::startInputProcess()
{
    while (true) {
        std::string input;
        std::getline(std::cin, input);

        if(isValid(input))
        {
            std::string processedInput = sortAndReplace(std::move(input));
            buffer.writeAndNotify(std::move(processedInput));
        }
    }
}


bool InputHandler::isValid(const std::string &s)
{
    return !s.empty() && s.length() <= 64 && std::all_of(s.begin(), s.end(), ::isdigit);
}


std::string InputHandler::sortAndReplace(std::string s)
{
    std::sort(s.rbegin(), s.rend());
    std::string newString;
    for(char c : s)
        if(c % 2 == 0)
            newString += "KB";
        else
            newString += c;
    return newString;
}

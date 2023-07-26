#pragma once
#include <mutex>
#include <condition_variable>


template<typename T>
class SafeBuffer : private std::mutex, private std::condition_variable
{
    bool newDataAvailable;
    T buffer;

public:
    SafeBuffer() : newDataAvailable(false) {}

    void writeAndNotify(T toBuffer)
    {
        std::unique_lock<std::mutex> lock(*this);
        buffer = std::move(toBuffer);
        newDataAvailable = true;
        lock.unlock();
        notify_all();
    }

    T readOnNotified()
    {
        std::unique_lock<std::mutex> lock(*this);
        wait(lock, [this]{ return newDataAvailable; });
        T bufferData = std::move(buffer);
        buffer.clear();
        newDataAvailable = false;
        lock.unlock();
        return bufferData;
    }
};

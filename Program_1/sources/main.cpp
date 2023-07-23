#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include <numeric>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

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

class SocketSystem{
    bool isConnected;
    int socket_ = -1;
    int listener = -1;
    int port;

public:

    void setListener()
    {
        listener = socket(AF_INET, SOCK_STREAM, 0);
        if (listener < 0)
        {
            perror("socket");
            exit(1);
        }
    }

    void bindSocket()
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("bind");
            exit(2);
        }
    }

    SocketSystem(int port) : port(port)
    {
        setListener();
        bindSocket();
        listen(listener, 1);
        setAutoReconnect();
    }

    void trySend(std::string num)
    {
        int bytes_read = send(socket_, num.c_str(), num.size(), 0);
//        close(socket_);
        std::cout << bytes_read << std::endl;
        if(bytes_read <= 0)
        {
            close(socket_);
            socket_ = -1;
            std::cout << "НЕ ОТПРАВЛЕНО\n";
        }else{
            std::cout << "ОТПРАВЛЕНО\n";
        }
    }
private:
    void setAutoReconnect()
    {
        std::thread reconnectionThread([&](){
            while(true)
            {
                if (socket_<0)
                {
                    std::cout << "ЖДУ ПОДКЛЮЧЕНИЯ\n";
                    socket_ = accept(listener, NULL, NULL);
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
        reconnectionThread.detach();
    }
};

class InputHandler
{
    SafeBuffer<std::string>& buffer;

public:
    InputHandler(SafeBuffer<std::string>& buffer) :
        buffer(buffer){}

    void startInputProcess()
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

private:
    bool isValid(const std::string& s)
    {
        return !s.empty() && s.length() <= 64 && std::all_of(s.begin(), s.end(), ::isdigit);
    }

    std::string sortAndReplace(std::string s)
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
};

class DataProcessor
{
    SafeBuffer<std::string>& buffer;

public:
    DataProcessor(SafeBuffer<std::string>& buffer) :
        buffer(buffer) {}

    void processAndTransfer(SocketSystem& socketSystem)
    {
        while (true) {
            std::string data = buffer.readOnNotified();
            if(!data.empty())
            {
                std::cout << data << std::endl;
                int sum = digitSum(std::move(data));

                socketSystem.trySend(std::to_string(sum));

            }
        }
    }

private:
    int digitSum(const std::string& s)
    {
        return std::accumulate(s.begin(), s.end(), 0, [](const int& sum, const char& c){
            return sum + (::isdigit(c) ? c - '0' : 0);
        });
    }
};


int main() {
    SafeBuffer<std::string> buffer;

    SocketSystem ss(52000);
    InputHandler handler(buffer);
    DataProcessor dp(buffer);

    std::thread inputThread(&InputHandler::startInputProcess, handler);
    std::thread processingThread(&DataProcessor::processAndTransfer, dp, std::ref(ss));

    inputThread.join();
    processingThread.join();

    return 0;
}

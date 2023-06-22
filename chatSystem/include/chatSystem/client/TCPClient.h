#pragma once
#include <boost/asio.hpp>
#include <queue>

namespace chat
{
    namespace io = boost::asio;
    using MessageHandler = std::function<void(std::string)>;

    class TCPClient
    {
    public:
        TCPClient(const std::string &address, const int port, const int q, const int n);

        void run();
        void stop();
        void post(const std::string &message);

    private:
        // String Messages
        void asyncRead();
        void onRead(boost::system::error_code ec, size_t bytesTransferred);
        void asyncWrite();
        void onWrite(boost::system::error_code ec, size_t bytesTransferred);

    public:
        MessageHandler onMessage;

    private:
        int _q;
        int _n;

        io::io_context _ioContext{};
        io::ip::tcp::socket _socket;

        io::ip::tcp::resolver::results_type _endpoints;

        io::streambuf _streamBuf{65536};
        std::queue<std::string> _outgoingMessages{};
    };
}
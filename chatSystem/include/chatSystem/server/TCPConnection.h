#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <queue>

namespace chat
{
    using boost::asio::ip::tcp;
    namespace io = boost::asio;

    using MessageHandler = std::function<void(std::string)>;
    using ErrorHandler = std::function<void()>;

    class TCPConnection : public std::enable_shared_from_this<TCPConnection>
    {
    public:
        using pointer = std::shared_ptr<TCPConnection>;
        static pointer create(io::ip::tcp::socket &&socket);
        tcp::socket &socket();
        void start(MessageHandler &&messageHandler, ErrorHandler &&errorHandler); // takes in callbacks as parameters
        void post(const std::string &message);                                    // send message to client
        inline const std::string &getUsername() const { return _username; };

    private:
        explicit TCPConnection(io::ip::tcp::socket &&socket);

        void asyncRead(); // wait for client's message
        void onRead(boost::system::error_code ec, size_t bytesTransferred);

        void asyncWrite();
        void onWrite(boost::system::error_code ec, size_t bytesTransferred);

    private:
        tcp::socket _socket;
        std::string _username;

        std::queue<std::string> _outgoingMessages;
        io::streambuf _streamBuf{65536};

        MessageHandler _messageHandler;
        ErrorHandler _errorHandler;
    };
}
#pragma once
#include <functional>
#include <unordered_set>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <chatSystem/server/TCPConnection.h>

namespace chat
{
    namespace io = boost::asio;

    enum class IPV
    {
        V4,
        V6
    };

    class TCPServer
    {
        using onJoinHandler = std::function<void(TCPConnection::pointer)>;
        using onLeaveHandler = std::function<void(TCPConnection::pointer)>;
        using onClientMessageHandler = std::function<void(std::string)>;

    public:
        TCPServer(IPV IPVersion, int port);

        int run();
        void broadcast(const std::string &message);

    public:
        onJoinHandler onJoin;
        onLeaveHandler onLeave;
        onClientMessageHandler onClientMessage;

    private:
        void startAccept();

    private:
        IPV _ipVersion;
        int _port;

        io::io_context _ioContext;
        io::ip::tcp::acceptor _acceptor;
        std::optional<io::ip::tcp::socket> _socket;
        std::unordered_set<TCPConnection::pointer> _connections{};
    };
}
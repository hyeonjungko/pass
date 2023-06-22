#include <chatSystem/server/TCPConnection.h>
#include <iostream>

namespace chat
{

    TCPConnection::TCPConnection(io::ip::tcp::socket &&socket) : _socket(std::move(socket))
    {
        boost::system::error_code ec;
        std::stringstream name;

        name << _socket.remote_endpoint();
        _username = name.str();
    }

    tcp::socket &TCPConnection::socket()
    {
        return _socket;
    }

    TCPConnection::pointer TCPConnection::create(io::ip::tcp::socket &&socket)
    {
        return pointer(new TCPConnection(std::move(socket)));
    }

    void TCPConnection::start(MessageHandler &&messageHandler, ErrorHandler &&errorHandler)
    {
        _messageHandler = std::move(messageHandler);
        _errorHandler = std::move(_errorHandler);
        asyncRead();
    }

    void TCPConnection::post(const std::string &message)
    {
        bool queueEmpty = _outgoingMessages.empty();

        _outgoingMessages.push(message);

        if (queueEmpty)
        {
            asyncWrite();
        }
    }

    void TCPConnection::asyncRead()
    {
        io::async_read_until(_socket, _streamBuf, "\n",
                             [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred)
                             {
                                 self->onRead(ec, bytesTransferred);
                             });
    }

    void TCPConnection::onRead(boost::system::error_code ec, size_t bytesTransferred)
    {
        //  Close socket if error on read
        if (ec)
        {
            _socket.close(ec);

            _errorHandler();
            return;
        }

        std::stringstream message;
        message << _username << ": " << std::istream(&_streamBuf).rdbuf();
        _streamBuf.consume(bytesTransferred);

        _messageHandler(message.str());

        asyncRead();
    }

    void TCPConnection::asyncWrite()
    {
        io::async_write(_socket, io::buffer(_outgoingMessages.front()),
                        [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred)
                        {
                            self->onWrite(ec, bytesTransferred);
                        });
    }

    void TCPConnection::onWrite(boost::system::error_code ec, size_t bytesTransferred)
    {
        // Close socket if error on write
        if (ec)
        {
            _socket.close(ec);

            _errorHandler();
            return;
        }

        _outgoingMessages.pop();

        if (!_outgoingMessages.empty())
        {
            asyncWrite();
        }
    }

}

#include <chatSystem/client/TCPClient.h>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>

namespace chat
{

    TCPClient::TCPClient(const std::string &address, const int port, const int q, const int n) : _socket(_ioContext), _q(q), _n(n)
    {
        std::cout << "Client initialized. Q: " << _q << " N: " << _n << std::endl;
        io::ip::tcp::resolver resolver{_ioContext};
        _endpoints = resolver.resolve(address, std::to_string(port));
    }

    void TCPClient::run()
    {
        io::async_connect(_socket, _endpoints,
                          [this](boost::system::error_code ec, io::ip::tcp::endpoint ep)
                          {
                              if (!ec)
                                  asyncRead();
                          });
        _ioContext.run();
    }

    void TCPClient::stop()
    {
        boost::system::error_code ec;
        _socket.close(ec);

        if (ec)
        {
            // process error
        }
    }

    void TCPClient::post(const std::string &message)
    {
        bool queueEmpty = _outgoingMessages.empty();
        _outgoingMessages.push(message);
        if (queueEmpty)
        {
            asyncWrite();
        }
    }

    void TCPClient::asyncRead()
    {
        io::async_read_until(_socket, _streamBuf, "\n",
                             [this](boost::system::error_code ec, size_t bytesTransferred)
                             {
                                 onRead(ec, bytesTransferred);
                             });
    }

    void TCPClient::onRead(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            stop();
            return;
        }

        std::stringstream message;
        message << std::istream{&_streamBuf}.rdbuf();
        onMessage(message.str());
        asyncRead();
    }

    void TCPClient::asyncWrite()
    {
        io::async_write(_socket,
                        io::buffer(_outgoingMessages.front()),
                        [this](boost::system::error_code ec, size_t bytesTransferred)
                        {
                            onWrite(ec, bytesTransferred);
                        });
    }

    void TCPClient::onWrite(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            stop();
            return;
        }

        _outgoingMessages.pop();

        if (!_outgoingMessages.empty())
        {
            asyncWrite();
        }
    }
}
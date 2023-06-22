#include <iostream>
#include <chatSystem/server/TCPServer.h>

int main(int argc, char *argv[])
{
    const int SERVER_PORT_NUM = 1337;
    chat::TCPServer server{chat::IPV::V4, SERVER_PORT_NUM};

    // add server handlers
    server.onJoin = [](chat::TCPConnection::pointer server)
    {
        std::cout << "A new client has joined! : " << server->getUsername() << std::endl;
    };

    server.onLeave = [](chat::TCPConnection::pointer server)
    {
        std::cout << "A client has left: " << server->getUsername() << std::endl;
    };

    server.onClientMessage = [&server](const std::string &message)
    {
        server.broadcast(message);
    };

    server.run();

    return 0;
}
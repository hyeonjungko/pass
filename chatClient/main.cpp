#include <cstdlib>
#include <iostream>
#include <thread>
#include <chatSystem/client/TCPClient.h>

using namespace chat;

using boost::asio::ip::tcp;

int main(int argc, char *argv[])
{
    const std::string SERVER_IP_ADDR = "127.0.0.1";
    const std::string SERVER_PORT = "1337";

    try
    {
        boost::asio::io_context ioContext;
        tcp::resolver resolver{ioContext};

        auto endpoints = resolver.resolve(SERVER_IP_ADDR, SERVER_PORT);

        tcp::socket socket{ioContext};
        boost::asio::connect(socket, endpoints);

        while (true)
        {
            // listen for messages
            std::array<char, 128> buf{};
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
            {
                break;
            }
            else if (error)
            {
                throw boost::system::system_error(error);
            }

            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
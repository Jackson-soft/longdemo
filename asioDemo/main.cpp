#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "hello world..." << std::endl;
    boost::asio::io_service io;
    boost::asio::ip::tcp::acceptor acceptor(
        io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8088));
    while (true) {
        boost::asio::ip::tcp::socket socketfd(io);
        acceptor.accept(socketfd);
        std::cout << socketfd.remote_endpoint().address() << std::endl;
        boost::system::error_code ec;
        socketfd.write_some(boost::asio::buffer("hello world!"));

        if (ec) {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            break;
        }
    }
    return 0;
}

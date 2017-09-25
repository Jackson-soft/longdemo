#include "msgs.pb.h"
#include <boost/asio.hpp>
#include <future>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    auto fu = std::async(std::launch::async,
                         []() { std::cout << "dfsdfa" << std::endl; });
    fu.get();
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    tutorial::Login loginMsg;
    loginMsg.set_name("fusu");
    loginMsg.set_password("123456");
    std::cout << "hello world..." << std::endl;
    boost::asio::io_service io;
    /*
    boost::asio::ip::tcp::acceptor acceptor(
        io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8088));
    */
    boost::asio::ip::tcp::acceptor acceptor(io);
    acceptor.open(boost::asio::ip::tcp::v4());

    int one = 1;
    setsockopt(acceptor.native_handle(),
               SOL_SOCKET,
               SO_REUSEADDR | SO_REUSEPORT,
               &one,
               sizeof(one));

    acceptor.bind(
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8088));
    acceptor.listen();
    while (true) {
        boost::asio::ip::tcp::socket socketfd(io);
        acceptor.accept(socketfd);
        std::cout << socketfd.remote_endpoint().address() << std::endl;
        while (true) {
            boost::system::error_code ec;

            boost::asio::streambuf r;
            boost::asio::read(socketfd, r);
            // std::cout << std::string(r.data()) << std::endl;

            boost::asio::streambuf b;
            std::ostream os(&b);
            loginMsg.SerializeToOstream(&os);
            boost::asio::write(socketfd, b);
            // socketfd.write_some(boost::asio::buffer("hello world!"));

            if (ec) {
                std::cout << boost::system::system_error(ec).what()
                          << std::endl;
                break;
            }
        }
    }
    return 0;
}

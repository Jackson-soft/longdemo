#include <cstdint>
#prag / noncopyable.hpp

#include "utils/noncopyable.hpp"
#include <chrono>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <unistd.h>

// 连接器
namespace Uranus::Net
{
// 连接器类
class Dialer : public Utils::Noncopyable
{
public:
    Dialer() = default;
    ~Dialer() { Close(); }

    //连接到网络地址
    auto Dial(std::string_view network, std::string_view ip, const std::uint16_t port) -> bool
    {
        if (network.empty() || ip.empty() || port <= 0) {
            return false;
        }
        mPort = port;

        if (!mSocket.NewSocket(network))
            return false;

        return mSocket.Connect(ip, port) == 0 ? true : false;
    }

    int Read() { return 0; }

    int Write() { return 0; }

    bool SetKeepAlive(bool on) { return mSocket.SetKeeplive(on) == 0 ? true : false; }

    void Close() { mSocket.Close(); }

    int Shutdown() { return mSocket.ShutDown(); }

private:
    // Socket对象
    Socket mSocket;

    std::chrono::duration<int> timeout;

    std::chrono::duration<int> keepAlive;
};
}  // namespace Uranus::Net

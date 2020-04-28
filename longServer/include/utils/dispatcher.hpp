#pragma once

#include "noncopyable.hpp"
#include <functional>
#include <google/protobuf/message.h>
#include <memory>
#include <unordered_map>

namespace Uranus::Utils
{
// 消息分发器
class Dispatcher : Noncopyable
{
public:
    using PBCallBack = std::function<void(const std::shared_ptr<google::protobuf::Message> &message)>;

public:
    Dispatcher()  = default;
    ~Dispatcher() = default;

    static std::shared_ptr<Dispatcher> Get()
    {
        static std::shared_ptr<Dispatcher> instance = std::make_shared<Dispatcher>();
        return instance;
    }

    // 消息注册
    void OnRegister(const google::protobuf::Message *message, const PBCallBack &callBack)
    {
        mCallBackMap[message->GetDescriptor()] = callBack;
    }

    // 消息分发
    void OnDispatcher(const std::shared_ptr<google::protobuf::Message> &message)
    {
        const auto it = mCallBackMap.find(message->GetDescriptor());
        if (it != mCallBackMap.end()) {
            it->second(message);
        }
    }

private:
    std::unordered_map<const google::protobuf::Descriptor *, PBCallBack> mCallBackMap;
};
}  // namespace Uranus::Utils
#pragma once

#include <functional>
#include <google/protobuf/message.h>
#include <memory>
#include <unordered_map>

namespace uranus::utils {
// 消息分发器
class Dispatcher {
public:
    using PBCallBack = std::function<void(const std::shared_ptr<google::protobuf::Message> &message)>;

    Dispatcher()  = default;
    ~Dispatcher() = default;

    static auto Get() -> std::shared_ptr<Dispatcher> {
        static std::shared_ptr<Dispatcher> instance = std::make_shared<Dispatcher>();
        return instance;
    }

    // 消息注册
    void Register(const google::protobuf::Message *message, const PBCallBack &callBack) {
        callBackMap_[message->GetDescriptor()] = callBack;
    }

    // 消息分发
    void Dispatch(const std::shared_ptr<google::protobuf::Message> &message) {
        const auto it = callBackMap_.find(message->GetDescriptor());
        if (it != callBackMap_.end()) {
            it->second(message);
        }
    }

private:
    std::unordered_map<const google::protobuf::Descriptor *, PBCallBack> callBackMap_{};
};
}  // namespace uranus::utils
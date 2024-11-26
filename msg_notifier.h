#pragma once

#include <memory>
#include <functional>
#undef min
#undef max
#include "dexode/EventBus.hpp"

namespace yk {
class MessageListener {
public:

    explicit MessageListener(const std::shared_ptr<dexode::EventBus::Listener>& l) : listener_(l) {}

    template<typename T>
    void Listen(std::function<void(const T&)>&& cbk) {
        listener_->listen([=](const T& msg) {
            cbk(msg);
        });
    }

private:
    std::shared_ptr<dexode::EventBus::Listener> listener_ = nullptr;
};

class MessageNotifier {
public:

    static std::shared_ptr<MessageNotifier> Make() {
        return std::make_shared<MessageNotifier>();
    }

    MessageNotifier();
    ~MessageNotifier();

    std::shared_ptr<MessageListener> CreateListener();

    template<typename T>
    void SendAppMessage(const T& m) {
        event_bus_->postpone(m);
        event_bus_->process();
    }

private:

    std::shared_ptr<dexode::EventBus> event_bus_ = nullptr;

};
}



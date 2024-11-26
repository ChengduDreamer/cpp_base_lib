
#include "msg_notifier.h"

namespace yk {

MessageNotifier::MessageNotifier() {
    event_bus_ = std::make_shared<dexode::EventBus>();
}

MessageNotifier::~MessageNotifier() {

}

std::shared_ptr<MessageListener> MessageNotifier::CreateListener() {
    auto inner = std::make_shared<dexode::EventBus::Listener>(event_bus_);
    auto listener = std::make_shared<MessageListener>(inner);
    return listener;
}

}

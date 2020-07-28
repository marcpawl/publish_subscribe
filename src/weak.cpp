
#include "weak.hpp"

#include <algorithm>

#include "limits.hpp"

namespace marcpawl::weak {

UpdateCounter::UpdateCounter(Publisher* publisher, int* updates)
    : subscription_(publisher->add_subscriber(this)), updates_(updates) {}

Subscription::Subscription(Subscriber* subscriber,
                           std::weak_ptr<Unsubscriber> unsubscriber)
    : subscriber_(subscriber), unsubscriber_(std::move(unsubscriber)) {}

Subscription::~Subscription() {
  std::shared_ptr<Unsubscriber> unsubscriber = unsubscriber_.lock();
  if (nullptr != unsubscriber) {
    unsubscriber->remove_subscriber(subscriber_);
  }
}

Unsubscriber::Unsubscriber(Publisher& publisher) : publisher_(publisher) {}

void Unsubscriber::remove_subscriber(Subscriber* subscriber) {
  publisher_.remove_subscriber(subscriber);
}

Publisher::Publisher() {
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  unsubscriber_.reset(new Unsubscriber(*this));
  subscribers_.reserve(reserved);
}

Subscription Publisher::add_subscriber(Subscriber* subscriber) {
  subscribers_.push_back(subscriber);
  return Subscription{subscriber, unsubscriber_};
}

void Publisher::remove_subscriber(Subscriber* subscriber) {
  auto new_end =
      std::remove(std::begin(subscribers_), std::end(subscribers_), subscriber);
  subscribers_.erase(new_end, std::end(subscribers_));
}

void Publisher::send_updates() {
  std::for_each(std::begin(subscribers_), std::end(subscribers_),
                [](Subscriber* subscriber) { subscriber->on_update(); });
}

}  // namespace marcpawl::weak

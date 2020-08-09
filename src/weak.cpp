
#include "weak.hpp"

#include <algorithm>
#include <utility>

namespace marcpawl::weak {

Counter::Counter(Publisher* publisher, int* updates)
    : publisher_(publisher), updates_(updates) {}

void Counter::on_update() { (*updates_)++; }

void Counter::start() {
  subscription_ = std::move(publisher_->subscribe(this));
}

void Counter::stop() { subscription_.reset(); }

Publisher::Unsubscriber::Unsubscriber(Publisher* publisher)
    : publisher_(publisher) {}

void Publisher::Unsubscriber::unsubscribe(Subscriber* subscriber) {
  publisher_->unsubscribe(subscriber);
}

Subscription::Subscription(std::weak_ptr<Publisher::Unsubscriber> unsubscriber,
                           Subscriber* subscriber)
    : unsubscriber_(std::move(unsubscriber)), subscriber_(subscriber) {}

Subscription::~Subscription() {
  auto locked = unsubscriber_.lock();
  if (nullptr != locked) {
    locked->unsubscribe(subscriber_);
  }
}

Publisher::Publisher() : unsubscriber_(std::make_shared<Unsubscriber>(this)) {
  subscribers_.reserve(reserved);
}

size_t Publisher::size() const noexcept { return subscribers_.size(); }

Subscription Publisher::subscribe(Subscriber* subscriber) noexcept {
  subscribers_.push_back(subscriber);
  return Subscription(unsubscriber_, subscriber);
}

void Publisher::unsubscribe(Subscriber* subscriber) noexcept {
  auto new_end =
      std::remove(std::begin(subscribers_), std::end(subscribers_), subscriber);
  subscribers_.erase(new_end, std::end(subscribers_));
}

void Publisher::update() {
  std::for_each(std::begin(subscribers_), std::end(subscribers_),
                [](Subscriber* subscriber) { subscriber->on_update(); });
}

}  // namespace marcpawl::weak

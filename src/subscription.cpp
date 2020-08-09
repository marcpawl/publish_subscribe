
#include "subscription.hpp"

#include <algorithm>
#include <utility>

namespace marcpawl::subscription {

Counter::Counter(Publisher* publisher, int* updates)
    : publisher_(publisher), updates_(updates) {}

void Counter::on_update() { (*updates_)++; }

void Counter::start() { subscription_ = publisher_->subscribe(this); }

void Counter::stop() { subscription_.reset(); }

Subscription::Subscription(Publisher* publisher, Subscriber* subscriber)
    : publisher_(publisher), subscriber_(subscriber) {}

Subscription::Subscription(Subscription&& other)
    : publisher_(other.publisher_), subscriber_(other.subscriber_) {
  other.publisher_ = nullptr;
  other.subscriber_ = nullptr;
}

Subscription::~Subscription() {
  if (nullptr != publisher_) {
    publisher_->unsubscribe(subscriber_);
  }
}

Subscription& Subscription::operator=(Subscription&& other) {
  publisher_ = other.publisher_;
  subscriber_ = other.subscriber_;
  other.publisher_ = nullptr;
  other.subscriber_ = nullptr;
  return *this;
}

Publisher::Publisher() { subscribers_.reserve(reserved); }

size_t Publisher::size() const noexcept { return subscribers_.size(); }

Subscription Publisher::subscribe(Subscriber* subscriber) noexcept {
  subscribers_.push_back(subscriber);
  return Subscription(this, subscriber);
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

}  // namespace marcpawl::subscription

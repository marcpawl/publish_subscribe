
#include "legacy.hpp"

#include <algorithm>

namespace marcpawl::legacy {

Counter::Counter(Publisher* publisher, int* updates)
    : publisher_(publisher), updates_(updates) {}

Counter::~Counter() {
  if (counting_) {
    publisher_->unsubscribe(this);
  }
}

void Counter::on_update() { (*updates_)++; }

void Counter::start() {
  if (!counting_) {
    publisher_->subscribe(this);
    counting_ = true;
  }
}

void Counter::stop() {
  if (counting_) {
    publisher_->unsubscribe(this);
    counting_ = false;
  }
}

Publisher::Publisher() { subscribers_.reserve(reserved); }

size_t Publisher::size() const noexcept { return subscribers_.size(); }

void Publisher::subscribe(Subscriber* subscriber) noexcept {
  subscribers_.push_back(subscriber);
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

}  // namespace marcpawl::legacy


#include "legacy.hpp"

#include <algorithm>

namespace marcpawl::legacy {

UpdateCounter::UpdateCounter(Publisher* publisher, int* updates)
    : publisher_(publisher), updates_(updates) {
  if (publisher_ != nullptr) {
    publisher_->add_subscriber(this);
  }
}

UpdateCounter::~UpdateCounter() {
  if (publisher_ != nullptr) {
    publisher_->remove_subscriber(this);
  }
}

Publisher::Publisher() { subscribers_.reserve(reserved); }

void Publisher::add_subscriber(Subscriber* subscriber) {
  subscribers_.push_back(subscriber);
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

}  // namespace marcpawl::legacy

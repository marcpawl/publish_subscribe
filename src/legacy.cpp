
#include "legacy.hpp"
#include <algorithm>

namespace marcpawl::legacy {


void Publisher::add_subscriber(Subscriber* subscriber)
{
  subscribers_.push_back(subscriber);
}

void Publisher::remove_subscriber(Subscriber* subscriber)
{
  auto new_end = std::remove(std::begin(subscribers_), std::end(subscribers_),
    subscriber);
  subscribers_.erase(new_end, std::end(subscribers_));
}

void Publisher::send_updates()
{
  std::for_each(std::begin(subscribers_), std::end(subscribers_),
    [](Subscriber* subscriber) { subscriber->on_update(); });
}


} // namespace marcpawl::legacy

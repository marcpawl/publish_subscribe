
#include "cadr.hpp"
#include <algorithm>

namespace marcpawl::cadr {

Subscription::~Subscription()
{
  publisher_.remove_subscriber(*this);
}

Subscription Publisher::add_subscriber(Subscriber* subscriber)
{
   subscribers_.push_back(subscriber);
   return Subscription{*subscriber, *this};
}

void Publisher::remove_subscriber(Subscription const& subscription)
{
  Subscriber* subscriber = &subscription.subscriber_;
  auto new_end = std::remove(std::begin(subscribers_), std::end(subscribers_),
    subscriber);
  subscribers_.erase(new_end, std::end(subscribers_));
}

void Publisher::send_updates()
{
  std::for_each(std::begin(subscribers_), std::end(subscribers_),
    [](Subscriber* subscriber) { subscriber->on_update(); });
}


} // namespace marcpawl::cadr

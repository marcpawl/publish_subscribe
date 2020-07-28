
#include "cadr.hpp"
#include <algorithm>

namespace marcpawl::cadr {

  UpdateCounter::UpdateCounter(Publisher* publisher, int* updates) 
  : subscription_(publisher->add_subscriber(this))
  , updates_(updates)
  {
  }


 Subscription::Subscription(Subscriber* subscriber, Publisher* publisher)
: subscriber_(subscriber)
, publisher_(publisher)
{}

Subscription::~Subscription()
{
  publisher_->remove_subscriber(*this);
}

  Publisher::Publisher() 
  {
    subscribers_.reserve(reserved);
  }

Subscription Publisher::add_subscriber(Subscriber* subscriber)
{
   subscribers_.push_back(subscriber);
   return Subscription{subscriber, this};
}

void Publisher::remove_subscriber(Subscription const& subscription)
{
  Subscriber* subscriber = subscription.subscriber_;
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

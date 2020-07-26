
#include "weak.hpp"
#include <algorithm>

namespace marcpawl::weak {

Subscription::~Subscription()
{
  std::shared_ptr<Unsubscriber> unsubscriber = unsubscriber_.lock();
  if (nullptr != unsubscriber) {
    unsubscriber->remove_subscriber(&subscriber_);
  }
}

void Unsubscriber::remove_subscriber(Subscriber* subscriber)
{
  publisher_.remove_subscriber(subscriber);
}


Publisher::Publisher()
  {
unsubscriber_ = std::make_shared<Unsubscriber>(*this);
    subscribers_.reserve(1024);
  }

Subscription Publisher::add_subscriber(Subscriber* subscriber)
{
   subscribers_.push_back(subscriber);
   return Subscription{*subscriber, unsubscriber_};
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


} // namespace marcpawl::weak

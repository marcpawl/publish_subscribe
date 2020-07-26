#pragma once
#include <vector>

namespace marcpawl::cadr {

class Publisher;
class Subscriber;

/** As long as the subscription still exists
  * the subscriber will be notified.
  */
class Subscription {
public:
  Subscription(Subscriber& subscriber, Publisher& publisher)
  : subscriber_(subscriber)
  , publisher_(publisher)
  {}

  Subscription(Subscription const&) = delete;
  Subscription(Subscription &&) = default;
  ~Subscription();
  Subscription& operator=(Subscription const&) = delete;
  Subscription& operator=(Subscription &&) = default;

private:
  Subscriber& subscriber_;
  Publisher&  publisher_;
  friend class Publisher;
};

class Subscriber {
public:
  virtual ~Subscriber() = default;
  /** Notification that on object being subscribed to has changed. */
  virtual void on_update() = 0;
};

class Publisher
{
public:
  Publisher() 
  {
    subscribers_.reserve(1024);
  }

  /** Start sending notifications when the object has changed in 
    * the future.  The notifications will be sent as long
    * as the subscription object is alive.
    * @param subscriber Object that will receive the notifications.
    */
  Subscription add_subscriber(Subscriber* subscriber);


  /** Send notifaction of an update to all the subscribers. */
  void send_updates();

private:
  std::vector<Subscriber*> subscribers_;

  void remove_subscriber(Subscription const& subscription);

  friend class Subscription;
};


class UpdateCounter : public Subscriber {
private:
  int updates_ = 0;
  Subscription subscription_;
  
public:
  UpdateCounter(Publisher* publisher) 
  : subscription_(publisher->add_subscriber(this))
  {
  }


  ~UpdateCounter() override = default;

  void on_update() override {
    updates_++;
  }
};


} // namespace marcpawl::cadr

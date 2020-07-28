#pragma once
#include <vector>
#include "limits.hpp"

namespace marcpawl::cadr {

class Publisher;
class Subscriber;

class Subscriber {
public:
  virtual ~Subscriber() = default;
  /** Notification that on object being subscribed to has changed. */
  virtual void on_update() = 0;
};

/** As long as the subscription still exists
  * the subscriber will be notified.
  */
class Subscription {
public:
  Subscription(Subscription const&) = delete;
  Subscription(Subscription &&) = default;
  ~Subscription();
  Subscription& operator=(Subscription const&) = delete;
  Subscription& operator=(Subscription &&) = default;

private:
  Subscription(Subscriber* subscriber, Publisher* publisher);
  Subscriber* subscriber_;
  Publisher*  publisher_;
  friend class Publisher;
};

class Publisher
{
public:
  Publisher();

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
  Subscription subscription_;
  int* const updates_;
  
public:
  UpdateCounter(Publisher* publisher, int* updates);
  ~UpdateCounter() override = default;

  void on_update() override
  {
    (*updates_)++;
  }
};


} // namespace marcpawl::cadr

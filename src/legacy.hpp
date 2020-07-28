#pragma once
#include <vector>
#include "limits.hpp"

namespace marcpawl::legacy {

class Subscriber {
public:
  virtual ~Subscriber() = default;
  /** Notification that on object being subscribed to has changed. */
  virtual void on_update() = 0;
};

class Publisher
{
public:
  Publisher();

  /** Start sending notifications when the object has changed in 
    * the future. {@link #remove_subscriber(Subscriber*)} must
    * be called prior to the subscriber being deleted.
    * @param subscriber Object that will receive the notifications.
    */
  void add_subscriber(Subscriber* subscriber);

  /** Stop sending notifications about object updates.
    * @param subscriber Object that will no longer receive notifications. */
  void remove_subscriber(Subscriber* subscriber);

  /** Send notifaction of an update to all the subscribers. */
  void send_updates();

private:
  std::vector<Subscriber*> subscribers_;
};


class UpdateCounter : public Subscriber {
private:
  Publisher* publisher_ = nullptr;
  int* const updates_;

public:
  UpdateCounter(Publisher* publisher, int* updates);
  ~UpdateCounter() override;

  void on_update() override
  {
    (*updates_)++;
  }
};


} // namespace marcpawl::legacy

#pragma once
#include <vector>

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
  Publisher() 
  {
    subscribers_.reserve(1024);
  }

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
  int updates_ = 0;
  Publisher* publisher_ = nullptr;

  //void* padding1_;
  //void* padding2_;
  
public:
  UpdateCounter(Publisher* publisher) 
  : publisher_(publisher)
  {
    if (publisher_ != nullptr) {
      publisher_->add_subscriber(this);
    }
  }


  ~UpdateCounter() override {
    if (publisher_ != nullptr) {
      publisher_->remove_subscriber(this);
    }
  }

  void on_update() override {
    updates_++;
  }
};


} // namespace marcpawl::legacy

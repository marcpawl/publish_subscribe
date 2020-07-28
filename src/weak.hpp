#pragma once
#include <memory>
#include <vector>

namespace marcpawl::weak {

class Publisher;
class Subscriber;
class Unsubscriber;

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
  Subscription(Subscription&&) = default;
  ~Subscription();
  Subscription& operator=(Subscription const&) = delete;
  Subscription& operator=(Subscription&&) = default;

 private:
  Subscription(Subscriber* subscriber,
               std::weak_ptr<Unsubscriber> unsubscriber);
  Subscriber* subscriber_;
  std::weak_ptr<Unsubscriber> unsubscriber_;
  friend class Unsubscriber;
  friend class Publisher;
};

class Unsubscriber {
 private:
  explicit Unsubscriber(Publisher& publisher);
  void remove_subscriber(Subscriber*);
  Publisher& publisher_;

  friend class Subscription;
  friend class Publisher;
};

class Publisher {
 private:
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
  std::shared_ptr<Unsubscriber> unsubscriber_;

  void remove_subscriber(Subscriber* subscriber);

  friend class Unsubscriber;
};

class UpdateCounter : public Subscriber {
 private:
  Subscription subscription_;
  int* updates_;

 public:
  UpdateCounter(Publisher* publisher, int* updates);
  ~UpdateCounter() override = default;

  void on_update() override { (*updates_)++; }
};

}  // namespace marcpawl::weak

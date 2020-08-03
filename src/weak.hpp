#pragma once
#include <memory>
#include <optional>
#include <vector>

#include "limits.hpp"

namespace marcpawl::weak {
class Publisher;
class Subscription;

class Subscriber {
 public:
  virtual ~Subscriber() = default;
  /** Notification that on object being subscribed to has changed. */
  virtual void on_update() = 0;
};

class Publisher {
 private:
  class Unsubscriber {
   private:
    Publisher* publisher_;

   public:
    Unsubscriber(Publisher* publisher);
    Unsubscriber(Unsubscriber const&) = delete;
    Unsubscriber(Unsubscriber&&) = default;
    ~Unsubscriber() = default;
    Unsubscriber& operator=(Unsubscriber const&) = delete;
    Unsubscriber& operator=(Unsubscriber&&) = default;

    void unsubscribe(Subscriber* subscriber);
  };

 public:
  Publisher();
  Publisher(Publisher const&) = delete;
  Publisher(Publisher&&) = default;
  ~Publisher() = default;
  Publisher& operator=(Publisher const&) = delete;
  Publisher& operator=(Publisher&&) = default;

  /** Start sending notifications when the object has changed in
   * the future.
   * @param subscriber Object that will receive the notifications.
   * As long as the subscription exists then the subscriber will
   * be notified.
   */
  Subscription subscribe(Subscriber* subscriber) noexcept;

 private:
  /** Stop sending notifications about object updates.
   * @param subscriber Object that will no longer receive notifications. */
  void unsubscribe(Subscriber* subscriber) noexcept;

 public:
  /** Send notifaction of an update to all the subscribers. */
  void update();

  /** @return Number of subscribers. */
  size_t size() const noexcept;

 private:
  std::shared_ptr<Unsubscriber> unsubscriber_;
  std::vector<Subscriber*> subscribers_;
  friend class Unsubscriber;
  friend class Subscription;
};

class Subscription {
 private:
  std::weak_ptr<Publisher::Unsubscriber> unsubscriber_;
  Subscriber* subscriber_;

  Subscription(std::weak_ptr<Publisher::Unsubscriber> unsubscriber_,
               Subscriber* subscriber);

 public:
  Subscription(Subscription const&) = delete;
  Subscription(Subscription&&) = default;
  ~Subscription();
  Subscription& operator=(Subscription const&) = delete;
  Subscription& operator=(Subscription&&) = default;

  friend class Publisher;
};

class Counter : public Subscriber {
 private:
  Publisher* const publisher_;
  std::optional<Subscription> subscription_;
  int* const updates_;

 public:
  Counter(Publisher* publisher, int* updates);
  Counter(Counter const&) = delete;
  Counter(Counter&&) = default;
  ~Counter() override = default;
  Counter& operator=(Counter const&) = delete;
  Counter& operator=(Counter&&) = default;

  void on_update() override;

  void start();
  void stop();
};

}  // namespace marcpawl::weak

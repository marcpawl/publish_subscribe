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

class Publisher {
 public:
  Publisher();
  Publisher(Publisher const&) = delete;
  Publisher(Publisher&&) = default;
  ~Publisher() = default;
  Publisher& operator=(Publisher const&) = delete;
  Publisher& operator=(Publisher&&) = default;

  /** Start sending notifications when the object has changed in
   * the future. {@link #remove_subscriber(Subscriber*)} must
   * be called prior to the subscriber being deleted.
   * @param subscriber Object that will receive the notifications.
   */
  void subscribe(Subscriber* subscriber) noexcept;

  /** Stop sending notifications about object updates.
   * @param subscriber Object that will no longer receive notifications. */
  void unsubscribe(Subscriber* subscriber) noexcept;

  /** Send notifaction of an update to all the subscribers. */
  void update();

  /** @return Number of subscribers. */
  size_t size() const noexcept;

 private:
  std::vector<Subscriber*> subscribers_;
};

class Counter : public Subscriber {
 private:
  Publisher* publisher_ = nullptr;
  bool counting_ = false;
  int* const updates_;

 public:
  Counter(Publisher* publisher, int* updates);
  Counter(Counter const&) = delete;
  Counter(Counter&&) = default;
  ~Counter() override;
  Counter& operator=(Counter const&) = delete;
  Counter& operator=(Counter&&) = default;

  void on_update() override;

  void start();
  void stop();
};

}  // namespace marcpawl::legacy

#include <catch2/catch.hpp>

#include "legacy.hpp"
#include "subscription.hpp"
#include "weak.hpp"

template <typename Publisher, typename Counter>
static void test_publisher_records_subscriptions() {
  int updates = 0;
  Publisher publisher;
  Counter counter(&publisher, &updates);
  counter.start();
  REQUIRE(publisher.size() == 1);
}

template <typename Publisher, typename Counter>
static void test_publisher_records_unsubscribe() {
  int updates = 0;
  Publisher publisher;
  Counter counter(&publisher, &updates);
  counter.start();
  counter.stop();
  REQUIRE(publisher.size() == 0);
}

template <typename Publisher, typename Counter>
static void test_subscriber_receives_updates() {
  int updates = 0;
  Publisher publisher;
  Counter counter(&publisher, &updates);
  counter.start();
  publisher.update();
  REQUIRE(updates == 1);
}

template <typename Publisher, typename Counter>
static void test_subscriber_stops_receiving_updates() {
  int updates = 0;
  Publisher publisher;
  Counter counter(&publisher, &updates);
  counter.start();
  publisher.update();
  counter.stop();
  REQUIRE(updates == 1);
}

template <typename Publisher, typename Counter>
static void test_subscriber_does_not_receive_updates_after_destruction() {
  int updates = 0;
  Publisher publisher;
  {
    Counter counter(&publisher, &updates);
    counter.start();
    publisher.update();
  }
  publisher.update();
  REQUIRE(updates == 1);
}

template <typename Publisher, typename Counter>
static void test_publisher_destructed_before_unsubscription() {
  int updates = 0;
  auto publisher = std::make_unique<Publisher>();
  auto counter = std::make_unique<Counter>(publisher.get(), &updates);
  counter->start();
  publisher.reset();
}

TEST_CASE("legacy publisher records subscriptions", "[legacy]") {
  using namespace marcpawl::legacy;
  test_publisher_records_subscriptions<Publisher, Counter>();
}

TEST_CASE("legacy publisher records unsubscribe", "[legacy]") {
  using namespace marcpawl::legacy;
  test_publisher_records_unsubscribe<Publisher, Counter>();
}

TEST_CASE("legacy subscriber receives updates", "[legacy]") {
  using namespace marcpawl::legacy;
  test_subscriber_receives_updates<Publisher, Counter>();
}

TEST_CASE("legacy subscriber stops receiving updates", "[legacy]") {
  using namespace marcpawl::legacy;
  test_subscriber_stops_receiving_updates<Publisher, Counter>();
}

TEST_CASE("legacy subscriber does not receive updates after destruction",
          "[legacy]") {
  using namespace marcpawl::legacy;
  test_subscriber_does_not_receive_updates_after_destruction<Publisher,
                                                             Counter>();
}

#if 0
TEST_CASE("legacy publisher cannot destruct before unsubscription", "[legacy]")
{
  using namespace marcpawl::legacy;
test_publisher_destructed_before_unsubscription<Publisher,Counter>();
}
#endif

TEST_CASE("subscription publisher records subscriptions", "[subscription]") {
  using namespace marcpawl::subscription;
  test_publisher_records_subscriptions<Publisher, Counter>();
}

TEST_CASE("subscription publisher records unsubscribe", "[subscription]") {
  using namespace marcpawl::subscription;
  test_publisher_records_unsubscribe<Publisher, Counter>();
}

TEST_CASE("subscription subscriber receives updates", "[subscription]") {
  using namespace marcpawl::subscription;
  test_subscriber_receives_updates<Publisher, Counter>();
}

TEST_CASE("subscription subscriber stops receiving updates", "[subscription]") {
  using namespace marcpawl::subscription;
  test_subscriber_stops_receiving_updates<Publisher, Counter>();
}

TEST_CASE("subscription subscriber does not receive updates after destruction",
          "[subscription]") {
  using namespace marcpawl::subscription;
  test_subscriber_does_not_receive_updates_after_destruction<Publisher,
                                                             Counter>();
}

#if 0
TEST_CASE("subscription publisher cannot destruct before unsubscription", "[subscription]")
{
  using namespace marcpawl::subscription;
test_publisher_destructed_before_unsubscription<Publisher,Counter>();
}
#endif

TEST_CASE("weak publisher records subscriptions", "[weak]") {
  using namespace marcpawl::weak;
  test_publisher_records_subscriptions<Publisher, Counter>();
}

TEST_CASE("weak publisher records unsubscribe", "[weak]") {
  using namespace marcpawl::weak;
  test_publisher_records_unsubscribe<Publisher, Counter>();
}

TEST_CASE("weak subscriber receives updates", "[weak]") {
  using namespace marcpawl::weak;
  test_subscriber_receives_updates<Publisher, Counter>();
}

TEST_CASE("weak subscriber stops receiving updates", "[weak]") {
  using namespace marcpawl::weak;
  test_subscriber_stops_receiving_updates<Publisher, Counter>();
}

TEST_CASE("weak subscriber does not receive updates after destruction",
          "[weak]") {
  using namespace marcpawl::weak;
  test_subscriber_does_not_receive_updates_after_destruction<Publisher,
                                                             Counter>();
}

TEST_CASE("weak publisher cannot destruct before unsubscription", "[weak]") {
  using namespace marcpawl::weak;
  test_publisher_destructed_before_unsubscription<Publisher, Counter>();
}

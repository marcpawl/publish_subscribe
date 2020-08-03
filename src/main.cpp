#include <benchmark/benchmark.h>

#include <cassert>
#include <memory>
#include <vector>

#include "legacy.hpp"
#include "subscription.hpp"
#include "weak.hpp"

template <typename Subscriber, typename Publisher>
static std::array<std::unique_ptr<Subscriber>, nb_counters> create_subscribers(
    Publisher& publisher, int* updates) {
  auto create_subscriber = [&publisher, updates]() {
    return std::make_unique<Subscriber>(&publisher, updates);
  };
  std::array<std::unique_ptr<Subscriber>, nb_counters> subscribers;
  std::generate(std::begin(subscribers), std::end(subscribers),
                create_subscriber);
  return subscribers;
}

template <typename Subscriber, typename Publisher>
static void benchmark_create(benchmark::State& state) {
  for (auto _ : state) {
    int updates = 0;
    Publisher publisher;
    create_subscribers<Subscriber, Publisher>(publisher, &updates);
  }
}

template <typename Subscriber, typename Publisher>
static void benchmark_destruct(benchmark::State& state) {
  state.PauseTiming();
  for (auto _ : state) {
    state.PauseTiming();
    int updates = 0;
    Publisher publisher;
    auto subscribers =
        create_subscribers<Subscriber, Publisher>(publisher, &updates);
    assert(subscribers.size() == nb_counters);
    state.ResumeTiming();
  }
}

template <typename Subscriber, typename Publisher>
static void benchmark_subscribe(benchmark::State& state) {
  state.PauseTiming();
  int updates = 0;
  for (auto _ : state) {
    state.PauseTiming();
    Publisher publisher;
    auto subscribers =
        create_subscribers<Subscriber, Publisher>(publisher, &updates);
    state.ResumeTiming();
    std::for_each(std::begin(subscribers), std::end(subscribers),
                  [](auto& subscriber) { subscriber->start(); });
    state.PauseTiming();
  }
}

template <typename Subscriber, typename Publisher>
static void benchmark_unsubscribe(benchmark::State& state) {
  state.PauseTiming();
  int updates = 0;
  for (auto _ : state) {
    state.PauseTiming();
    Publisher publisher;
    auto subscribers =
        create_subscribers<Subscriber, Publisher>(publisher, &updates);
    std::for_each(std::begin(subscribers), std::end(subscribers),
                  [](auto& subscriber) { subscriber->start(); });
    state.ResumeTiming();
    std::for_each(std::begin(subscribers), std::end(subscribers),
                  [](auto& subscriber) { subscriber->stop(); });
    state.PauseTiming();
  }
}

template <typename Subscriber, typename Publisher>
static void benchmark_update(benchmark::State& state) {
  int updates = 0;
  Publisher publisher;
  auto subscribers =
      create_subscribers<Subscriber, Publisher>(publisher, &updates);

  for (auto _ : state) {
    publisher.update();
  }
  assert(updates > 0);
}

static void A_create_legacy(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_create<Counter, Publisher>(state);
}

static void B_destruct_legacy(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_destruct<Counter, Publisher>(state);
}

static void C_subscribe_legacy(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_subscribe<Counter, Publisher>(state);
}

static void D_unsubscribe_legacy(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_unsubscribe<Counter, Publisher>(state);
}

static void E_update_legacy(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_update<Counter, Publisher>(state);
}

static void A_create_subscription(benchmark::State& state) {
  using namespace marcpawl::subscription;
  benchmark_create<Counter, Publisher>(state);
}

static void B_destruct_subscription(benchmark::State& state) {
  using namespace marcpawl::subscription;
  benchmark_destruct<Counter, Publisher>(state);
}

static void C_subscribe_subscription(benchmark::State& state) {
  using namespace marcpawl::subscription;
  benchmark_subscribe<Counter, Publisher>(state);
}

static void D_unsubscribe_subscription(benchmark::State& state) {
  using namespace marcpawl::subscription;
  benchmark_unsubscribe<Counter, Publisher>(state);
}

static void E_update_subscription(benchmark::State& state) {
  using namespace marcpawl::subscription;
  benchmark_update<Counter, Publisher>(state);
}

static void A_create_weak(benchmark::State& state) {
  using namespace marcpawl::weak;
  benchmark_create<Counter, Publisher>(state);
}

static void B_destruct_weak(benchmark::State& state) {
  using namespace marcpawl::weak;
  benchmark_destruct<Counter, Publisher>(state);
}

static void C_subscribe_weak(benchmark::State& state) {
  using namespace marcpawl::weak;
  benchmark_subscribe<Counter, Publisher>(state);
}

static void D_unsubscribe_weak(benchmark::State& state) {
  using namespace marcpawl::weak;
  benchmark_unsubscribe<Counter, Publisher>(state);
}

static void E_update_weak(benchmark::State& state) {
  using namespace marcpawl::weak;
  benchmark_update<Counter, Publisher>(state);
}

// Register the function as a benchmark
BENCHMARK(A_create_legacy);       // NOLINT (cert-err58-cpp)
BENCHMARK(B_destruct_legacy);     // NOLINT (cert-err58-cpp)
BENCHMARK(C_subscribe_legacy);    // NOLINT (cert-err58-cpp)
BENCHMARK(D_unsubscribe_legacy);  // NOLINT (cert-err58-cpp)
BENCHMARK(E_update_legacy);       // NOLINT (cert-err58-cpp)

BENCHMARK(A_create_subscription);       // NOLINT (cert-err58-cpp)
BENCHMARK(B_destruct_subscription);     // NOLINT (cert-err58-cpp)
BENCHMARK(C_subscribe_subscription);    // NOLINT (cert-err58-cpp)
BENCHMARK(D_unsubscribe_subscription);  // NOLINT (cert-err58-cpp)
BENCHMARK(E_update_subscription);       // NOLINT (cert-err58-cpp)

BENCHMARK(A_create_weak);       // NOLINT (cert-err58-cpp)
BENCHMARK(B_destruct_weak);     // NOLINT (cert-err58-cpp)
BENCHMARK(C_subscribe_weak);    // NOLINT (cert-err58-cpp)
BENCHMARK(D_unsubscribe_weak);  // NOLINT (cert-err58-cpp)
BENCHMARK(E_update_weak);       // NOLINT (cert-err58-cpp)

// Run the benchmark
BENCHMARK_MAIN();

#include <benchmark/benchmark.h>

#include <cassert>
#include <memory>
#include <vector>

#include "legacy.hpp"

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
    create_subscribers<Subscriber,Publisher>(publisher, &updates);
  }
}

template <typename Subscriber, typename Publisher>
static void benchmark_destruct(benchmark::State& state) {
  state.PauseTiming();
  for (auto _ : state) {
    state.PauseTiming();
    int updates = 0;
    Publisher publisher;
    auto subscribers = create_subscribers<Subscriber,Publisher>(publisher, &updates);
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
    auto subscribers = create_subscribers<Subscriber, Publisher>(publisher, &updates);
    state.ResumeTiming();
    std::for_each(std::begin(subscribers), std::end(subscribers), 
[](auto& subscriber) { subscriber->start(); } );
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
    auto subscribers = create_subscribers<Subscriber, Publisher>(publisher, &updates);
    std::for_each(std::begin(subscribers), std::end(subscribers), 
[](auto& subscriber) { subscriber->start(); } );
    state.ResumeTiming();
    std::for_each(std::begin(subscribers), std::end(subscribers), 
[](auto& subscriber) { subscriber->stop(); } );
    state.PauseTiming();
  }
}

template <typename Subscriber, typename Publisher>
static void benchmark_update(benchmark::State& state) {
  int updates = 0;
  Publisher publisher;
  auto subscribers = create_subscribers<Subscriber, Publisher>(publisher, &updates);

  for (auto _ : state) {
    publisher.update();
  }
  assert(updates > 0);
}

static void legacy_create(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_create<Counter, Publisher>(state);
}

static void legacy_destruct(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_destruct<Counter, Publisher>(state);
}

static void legacy_subscribe(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_subscribe<Counter, Publisher>(state);
}

static void legacy_unsubscribe(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_unsubscribe<Counter, Publisher>(state);
}

static void legacy_update(benchmark::State& state) {
  using namespace marcpawl::legacy;
  benchmark_update<Counter, Publisher>(state);
}


// Register the function as a benchmark
BENCHMARK(legacy_create);  // NOLINT (cert-err58-cpp)
BENCHMARK(legacy_destruct);  // NOLINT (cert-err58-cpp)
BENCHMARK(legacy_subscribe);  // NOLINT (cert-err58-cpp)
BENCHMARK(legacy_unsubscribe);  // NOLINT (cert-err58-cpp)
BENCHMARK(legacy_update);     // NOLINT (cert-err58-cpp)

// Run the benchmark
BENCHMARK_MAIN();

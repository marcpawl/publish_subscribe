#include <benchmark/benchmark.h>
#include <memory>
#include <vector>
#include "legacy.hpp"
#include "cadr.hpp"
#include "weak.hpp"

constexpr static int nb_counters = 1'000;


static void legacy_subscribe(benchmark::State& state) {
  using namespace marcpawl::legacy;
  std::array<std::unique_ptr<UpdateCounter>, nb_counters> subscribers;
  Publisher publisher;
  for (auto _ : state) {
    for (auto& subscriber : subscribers ) {
      subscriber = std::make_unique<UpdateCounter>(&publisher);
    }
  }
}

static void cadr_subscribe(benchmark::State& state) {
  using namespace marcpawl::cadr;
  std::array<std::unique_ptr<UpdateCounter>, nb_counters> subscribers;
  Publisher publisher;
  for (auto _ : state) {
    for (auto& subscriber : subscribers ) {
      subscriber = std::make_unique<UpdateCounter>(&publisher);
    }
  }
}

static void weak_subscribe(benchmark::State& state) {
  using namespace marcpawl::weak;
  std::array<std::unique_ptr<UpdateCounter>, nb_counters> subscribers;
  Publisher publisher;
  for (auto _ : state) {
    for (auto& subscriber : subscribers ) {
      subscriber = std::make_unique<UpdateCounter>(&publisher);
    }
  }
}

static void legacy_update(benchmark::State& state) {
  using namespace marcpawl::legacy;
  std::array<std::unique_ptr<UpdateCounter>, nb_counters> subscribers;
  Publisher publisher;
  for (auto& subscriber : subscribers ) {
    subscriber = std::make_unique<UpdateCounter>(&publisher);
  }
  for (auto _ : state) {
    publisher.send_updates();
  }
}


static void cadr_update(benchmark::State& state) {
  using namespace marcpawl::cadr;
  std::array<std::unique_ptr<UpdateCounter>, nb_counters> subscribers;
  Publisher publisher;
  for (auto& subscriber : subscribers ) {
    subscriber = std::make_unique<UpdateCounter>(&publisher);
  }
  for (auto _ : state) {
    publisher.send_updates();
  }
}

static void weak_update(benchmark::State& state) {
  using namespace marcpawl::weak;
  std::array<std::unique_ptr<UpdateCounter>, nb_counters> subscribers;
  Publisher publisher;
  for (auto& subscriber : subscribers ) {
    subscriber = std::make_unique<UpdateCounter>(&publisher);
  }
  for (auto _ : state) {
    publisher.send_updates();
  }
}

// Register the function as a benchmark
BENCHMARK(legacy_subscribe);
BENCHMARK(cadr_subscribe);
BENCHMARK(weak_subscribe);
BENCHMARK(legacy_update);
BENCHMARK(cadr_update);
BENCHMARK(weak_update);
// Run the benchmark
BENCHMARK_MAIN();

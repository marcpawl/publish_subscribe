#include <benchmark/benchmark.h>
#include <memory>
#include <vector>
#include "legacy.hpp"

constexpr int nb_counters = 1'000;

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
// Register the function as a benchmark
BENCHMARK(legacy_subscribe);
// Run the benchmark
BENCHMARK_MAIN();

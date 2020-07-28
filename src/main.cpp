#include <benchmark/benchmark.h>
#include <memory>
#include <vector>
#include "legacy.hpp"
#include "cadr.hpp"
#include "weak.hpp"

template <typename Subscriber, typename Publisher>
static std::array<std::unique_ptr<Subscriber>, nb_counters> 
create_subscribers(Publisher& publisher, int* updates)
{
  auto create_subscriber = [&publisher, updates]()
  {
    return std::make_unique<Subscriber>(&publisher, updates);
  };
  std::array<std::unique_ptr<Subscriber>, nb_counters> subscribers;
  std::generate( std::begin(subscribers), std::end(subscribers),
    create_subscriber);
  return subscribers;
}


template <typename Subscriber, typename Publisher>
static void subscribe(benchmark::State & state) 
{
  int updates = 0;
  Publisher publisher;
  for (auto _ : state) {
  create_subscribers<Subscriber,Publisher>(publisher, &updates);
}
}

template <typename Subscriber, typename Publisher>
static void update(benchmark::State & state)  
{
  int updates = 0;
  Publisher publisher;
  create_subscribers<Subscriber,Publisher>(publisher, &updates);

  for (auto _ : state) {
    publisher.send_updates();
  }
}


static void legacy_subscribe(benchmark::State& state) {
  using namespace marcpawl::legacy ;
  subscribe<UpdateCounter, Publisher>(state);
}

static void cadr_subscribe(benchmark::State& state) {
  using namespace marcpawl::cadr;
  subscribe<UpdateCounter, Publisher>(state);
}

static void weak_subscribe(benchmark::State& state) {
  using namespace marcpawl::weak;
  subscribe<UpdateCounter, Publisher>(state);
}

static void legacy_update(benchmark::State& state) {
  using namespace marcpawl::legacy;
  update<UpdateCounter, Publisher>(state);
}

static void cadr_update(benchmark::State& state) {
  using namespace marcpawl::cadr;
  update<UpdateCounter, Publisher>(state);
}

static void weak_update(benchmark::State& state) {
  using namespace marcpawl::weak;
  update<UpdateCounter, Publisher>(state);
}

// Register the function as a benchmark
BENCHMARK(legacy_subscribe); // NOLINT (cert-err58-cpp)
BENCHMARK(legacy_update); // NOLINT (cert-err58-cpp)
BENCHMARK(cadr_subscribe); // NOLINT (cert-err58-cpp)
BENCHMARK(cadr_update); // NOLINT (cert-err58-cpp)
BENCHMARK(weak_subscribe); // NOLINT (cert-err58-cpp)
BENCHMARK(weak_update); // NOLINT (cert-err58-cpp)
// Run the benchmark
BENCHMARK_MAIN();

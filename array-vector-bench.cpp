#include <cstdint>
#include <array>
#include <vector>

static void VectorLoop(benchmark::State& state) {
    for (auto _ : state) {
      std::vector<int> vec = {0};
      for (int i = 0; i < 5; ++i) {
          vec.push_back(i);
          volatile int x = vec[i];
          benchmark::DoNotOptimize(x);
      }
    }
}

static void VectorLoopCached(benchmark::State& state) {
    for (auto _ : state) {
      std::vector<int> vec = {0};
      vec.reserve(5);

      for (size_t i = 0; i < 5; ++i) {
          vec.push_back(i);
          volatile int x = vec[i];
          benchmark::DoNotOptimize(x);
      }
    }
}

static void ArrayLoop(benchmark::State& state) {
    for (auto _ : state) {
      std::array<int, 5> arr = {1, 2, 3, 4, 5};

      for (size_t i = 0; i < 5; ++i) {
          arr[i] = i;
          volatile int x = arr[i];
          benchmark::DoNotOptimize(x);
      }
    }
}

static void EmptyLoop(benchmark::State& state) {
    for (auto _ : state) {
      std::array<int, 5> arr = {1, 2, 3, 4, 5};

      for (size_t i = 0; i < arr.size(); ++i) {
          volatile int x = arr[i];
          benchmark::DoNotOptimize(x);
      }
    }
}

BENCHMARK(VectorLoop);
BENCHMARK(VectorLoopCached);
BENCHMARK(ArrayLoop);
BENCHMARK(EmptyLoop);

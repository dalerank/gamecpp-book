constexpr size_t MIN_SIZE = 2;
constexpr size_t MAX_SIZE = 1024;

static void BM_Map(benchmark::State& state) {
  for (auto _ : state) {
    std::map<int64_t, int64_t> map;
    for (int64_t i = 0; i < state.range(0); ++i) {
      map[i] = i;
    }
    benchmark::DoNotOptimize(map);
  }
}
BENCHMARK(BM_Map)->Range(MIN_SIZE, MAX_SIZE);

static void BM_Umap(benchmark::State& state) {
  for (auto _ : state) {
    std::unordered_map<int64_t, int64_t> arr{};
    for (int64_t i = 0; i < state.range(0); ++i) {
      arr[i] = i;
    }
    benchmark::DoNotOptimize(arr);
  }
}
BENCHMARK(BM_Umap)->Range(MIN_SIZE, MAX_SIZE);

static void BM_F14FastMap(benchmark::State& state) {
  for (auto _ : state) {
    folly::F14FastMap<int64_t, int64_t> map;
    for (int64_t i = 0; i < state.range(0); ++i) {
      map[i] = i;
    }
    benchmark::DoNotOptimize(map);
  }
}
BENCHMARK(BM_F14FastMap)->Range(MIN_SIZE, MAX_SIZE);


static void BM_PMRSet(benchmark::State& state) {
  std::array<std::byte, MAX_SIZE * sizeof(int64_t)> buffer;
  std::pmr::monotonic_buffer_resource pool(buffer.data(), buffer.size());

  for (auto _ : state) {
    pool.release();
    std::pmr::unordered_set<int64_t> map{&pool};
    map.reserve(state.range(0));
    for (int64_t i = 0; i < state.range(0); ++i) {
      map.insert(i);
    }
    benchmark::DoNotOptimize(map);
  }
}
BENCHMARK(BM_PMRSet)->Range(MIN_SIZE, MAX_SIZE);

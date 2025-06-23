static void BM_std_string(benchmark::State& state) {
  for (auto _ : state) {
    std::string logMessage;
    logMessage.resize(256);
    logMessage = "Game Log: Player X has entered the level!";
    benchmark::DoNotOptimize(logMessage);
  }
}
BENCHMARK(BM_std_string);

// Benchmark для std::pmr::string
static void BM_pmr_string(benchmark::State& state) {
  for (auto _ : state) {
    char buffer[256];
    std::pmr::monotonic_buffer_resource pool{buffer, sizeof(buffer)};
    std::pmr::string logMessage(&pool);

    logMessage = "Game Log: Player X has entered the level!";
    benchmark::DoNotOptimize(logMessage);
  }
}
BENCHMARK(BM_pmr_string);

// Benchmark для char buffer
static void BM_char_buffer(benchmark::State& state) {
  for (auto _ : state) {
    char buffer[256];

    strcpy(buffer, "Game Log: Player X has entered the level!");
    benchmark::DoNotOptimize(buffer);
  }
}
BENCHMARK(BM_char_buffer);

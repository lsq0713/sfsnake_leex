#include <benchmark/benchmark.h>
#include "main.h" // 修改为实际 main 函数所在的文件路径

void runMain(); // 声明 runMain 函数

// 基准测试 runMain 函数
static void BM_RunMain(benchmark::State& state) {
    for (auto _ : state) {
        runMain();
    }
}
BENCHMARK(BM_RunMain);

BENCHMARK_MAIN();

// This file defines the interface and types needed when parsing the JSON
// configuration files used by runner.c.
#ifndef PARSE_CONFIG_H
#define PARSE_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define USE_DEFAULT_CPU_CORE (-1)
#define USE_DEFAULT_STREAM_PRIORITY (-100)

// Holds the configuration for a single benchmark, as specified by the JSON
// format given in the README.
typedef struct {
  // The name of the benchmark's .so file
  char *filename;
  // The name of the benchmark's JSON log, relative to base_result_directory.
  // If NULL, a log name will be used based on the benchmark's filename.
  char *log_name;
  // An extra label or name for the benchmark, included in its JSON log file.
  char *label;
  // The number of threads to request the benchmark to use
  int thread_count;
  // The number of blocks to request the benchmark to use
  int block_count;
  // The size, in bytes, of the input data the benchmark should generate or use
  uint64_t data_size;
  // A string containing an additional user-defined argument to pass to the
  // benchmark during initialization. May be either NULL or empty if
  // unspecified.
  char *additional_info;
  // The maximum number of iterations for this benchmark alone; will override
  // the global limit if set (0 = unlimited, negative = unset).
  int64_t max_iterations;
  // The maximum number of seconds to run this benchmark alone; will override
  // the global limit if set (0 = unlimited, negative = unset).
  double max_time;
  // The number of seconds for which the benchmark should sleep before
  // starting. If 0 or negative, it won't sleep at all.
  double release_time;
  // The CPU core to pin this benchmark to. Ignored if negative.
  int cpu_core;
  // The stream priority used to create the CUDA stream. Ignored if negative.
  int stream_priority;
} SingleBenchmarkConfiguration;

// Holds default settings for all benchmarks, and a list of individual
// benchmarks with their specific settings.
typedef struct {
  // The default cap on the number of iterations each benchmark will run.
  // Unlimited if 0 or lower.
  int64_t max_iterations;
  // The default cap on the number of seconds each benchmark will run.
  // Unlimited if 0 or lower.
  double max_time;
  // Set to 0 if each benchmark should be run in a separate thread. Set to 1 if
  // each should be run in a child process instead.
  int use_processes;
  // The CUDA device to run benchmarks on.
  int cuda_device;
  // The path to the base directory in which benchmark's log files are stored.
  char *base_result_directory;
  // The name of the scenario being tested.
  char *scenario_name;
  // If zero, CPU assignment is either handled by the system or taken from each
  // benchmark's cpu_core setting. If nonzero, benchmarks are distributed
  // evenly accrorss CPU cores.
  int pin_cpus;
  // The number of entries in the benchmarks list. Must never be 0.
  int benchmark_count;
  // The list of benchmarks to run.
  SingleBenchmarkConfiguration *benchmarks;
} GlobalConfiguration;

// Parses a JSON config file, and allocates and returns a GlobalConfiguration
// struct. Returns NULL on error. When no longer needed, the returned pointer
// must be passed to FreeGlobalConfiguration(...) to free resources. May print
// a message to stdout on error.
GlobalConfiguration* ParseConfiguration(const char *filename);

void FreeGlobalConfiguration(GlobalConfiguration *config);

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // PARSE_CONFIG_H

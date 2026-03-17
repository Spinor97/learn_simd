#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <sstream>
#include "csv_builder.h"

using namespace std;
using namespace std::chrono;

// ------------------------------------------------------------
// Generate synthetic CSV
// ------------------------------------------------------------
std::string generate_csv(size_t rows, size_t cols)
{
    std::string out;
    out.reserve(rows * cols * 16);   // rough estimate

    std::mt19937_64 rng(123);
    std::uniform_int_distribution<int64_t> dist_int(0, 1000000);
    std::uniform_real_distribution<double> dist_double(0.0, 1000000.0);

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {

            if (c % 3 == 0) {
                out += std::to_string(dist_int(rng));
            }
            else if (c % 3 == 1) {
                out += std::to_string(dist_double(rng));
            }
            else {
                out += "str_";
                out += std::to_string(dist_int(rng));
            }

            if (c + 1 != cols)
                out += ',';
        }
        out += '\n';
    }

    return out;
}

// ------------------------------------------------------------
// Benchmark runner
// ------------------------------------------------------------
void run_benchmark(size_t rows, size_t cols)
{
    cout << "Generating CSV...\n";
    std::string csv = generate_csv(rows, cols);

    double size_mb = csv.size() / (1024.0 * 1024.0);
    cout << "CSV size: " << size_mb << " MB\n";

    // Define schema
    vector<colType> schema;
    for (size_t c = 0; c < cols; ++c) {
        if (c % 3 == 0)
            schema.push_back(colType::Int64);
        else if (c % 3 == 1)
            schema.push_back(colType::Double);
        else
            schema.push_back(colType::String);
    }

    // Warmup
    {
        CSVBuilder warmup(csv, schema);
        auto table = warmup.build();
    }

    cout << "Running benchmark...\n";

    constexpr int iterations = 5;
    double best_time = 1e9;

    for (int i = 0; i < iterations; ++i) {
        auto start = steady_clock::now();

        CSVBuilder builder(csv, schema);
        auto table = builder.build();

        auto end = steady_clock::now();
        double seconds =
            duration_cast<duration<double>>(end - start).count();

        best_time = std::min(best_time, seconds);

        cout << "Run " << i+1
             << ": " << seconds << " sec\n";
    }

    double throughput = size_mb / best_time;

    cout << "\n===== RESULTS =====\n";
    cout << "Best Time: " << best_time << " sec\n";
    cout << "Throughput: " << throughput << " MB/s\n";
}
 
// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main()
{
    size_t rows = 1'000'000;
    size_t cols = 6;

    run_benchmark(rows, cols);

    return 0;
}
Overview(ai generated)
This project implements a high‑throughput CSV ingestion engine in C++ optimized for structured trading market data.

It targets fixed‑schema quote data of the form:

bid_price,bid_volume,ask_price,ask_volume,condition_code

The focus of this project is systems‑level performance engineering, not general CSV compliance.

On an AMD Ryzen 9 7950X, the parser achieves:

~739 MB/s single‑thread throughput

Motivation
Market data ingestion is a core component of:

Backtesting engines
Order book reconstruction
Historical replay systems
Quant research pipelines
Most CSV parsers optimize for flexibility and correctness.
This implementation instead optimizes for:

Fixed schema
Trusted input format
Numeric‑heavy workloads
Maximum per‑core throughput
The goal is to explore performance tradeoffs and CPU‑aware optimization techniques.

Design Highlights
Fixed Schema
The parser assumes a known column layout:

Column	Type
bid_price	double
bid_volume	int
ask_price	double
ask_volume	int
condition_code	int
This eliminates:

Dynamic type detection
Virtual dispatch
Runtime schema parsing
Columnar Memory Layout
Data is stored in column vectors, for example:

cpp
std::vector<double> bid_price;
std::vector<int> bid_volume;
std::vector<double> ask_price;
std::vector<int> ask_volume;
std::vector<int> condition_code;

Benefits:

Better cache locality
Backtester‑ready format
SIMD‑friendly layout
No per‑row struct overhead
Benchmark
Hardware
CPU: AMD Ryzen 9 7950X (16C / 32T, Zen 4)
Max Boost: 5.88 GHz
L3 Cache: 64 MB
Architecture: x86_64
SIMD: AVX2 / AVX512 supported
Software
Compiler: GCC
Flags: -O3 -march=native -std=c++20
OS: Linux
Dataset
60 MB synthetic trading CSV
5 fixed numeric columns
No strings or quotes
Result
Single‑thread throughput:

~739 MB/s

Estimated performance:

~7.4 cycles per byte (compute-bound)

CPU governor testing showed no measurable difference between schedutil and performance, indicating the workload was already frequency boosted.

Build
bash
g++ -O3 -march=native -std=c++20 parser.cpp -o parser

Run:

bash
./parser input.csv

Future Improvements
SIMD (AVX2 / AVX512) delimiter scanning
Multithreaded file partitioning
Memory‑mapped I/O
Store prices as fixed‑point integers (ticks)
Benchmark comparison vs pandas / Polars
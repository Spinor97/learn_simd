#include <cstring>
#include <cstdio>
#include <chrono>
#include "line_parser.hpp"
#include "ord_parser.hpp"

int main() {
    const char* msg = "AAPL,150.25,100\n";
    size_t len = strlen(msg);
    const size_t num_iterations = 1000000;

    parsedMsg parsed;
    vector<int> commas;
    vector<int> newlines;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_iterations; ++i) {
        parse_ordinary(std::string(msg), parsed);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    printf("Ordinary parsing time: %.6f seconds\n", elapsed.count());

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_iterations; ++i) {
        parsed_fix_msg(msg, len, parsed, commas, newlines);
    }

    end = std::chrono::high_resolution_clock::now();

    elapsed = end - start;
    printf("FIX parsing time: %.6f seconds\n", elapsed.count());

    //if (parsed_fix_msg(msg, len, parsed, commas, newlines)) {
        // Successfully parsed
        //printf("Symbol: %.*s\n", parsed.symLen, parsed.sym);
        //printf("Price: %.*s\n", parsed.pxLen, parsed.px);
        //printf("Quantity: %.*s\n", parsed.qtyLen, parsed.qty);
    //} else {
        // Parsing failed
        //printf("Failed to parse message.\n");
    //}
}
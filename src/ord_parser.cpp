#include "ord_parser.hpp"

void parse_ordinary(const std::string& msg, parsedMsg& out) {
    const char* data = msg.c_str();
    const char* start = data;
    const char* end = data;

    // Symbol
    while (*end != ',') ++end;
    out.sym = start;
    out.symLen = end - start;

    // Price
    start = ++end;
    while (*end != ',') ++end;
    out.px = start;
    out.pxLen = end - start;

    // Quantity
    start = ++end;
    while (*end != '\n') ++end;
    out.qty = start;
    out.qtyLen = end - start;
}
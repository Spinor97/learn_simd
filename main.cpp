#include "line_parser.cpp"

int main() {
    const char* msg = "AAPL,150.25,100\n";
    size_t len = strlen(msg);

    parsedMsg parsed;
    vector<int> commas;
    vector<int> newlines;

    if (parsed_fix_msg(msg, len, parsed, commas, newlines)) {
        // Successfully parsed
        printf("Symbol: %.*s\n", parsed.symLen, parsed.sym);
        printf("Price: %.*s\n", parsed.pxLen, parsed.px);
        printf("Quantity: %.*s\n", parsed.qtyLen, parsed.qty);
    } else {
        // Parsing failed
        printf("Failed to parse message.\n");
    }
}
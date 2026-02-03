#pragma once
#include <cstdint>

struct parsedMsg {
    const char* sym;
    uint8_t symLen;

    const char* px;
    uint8_t pxLen;

    const char* qty;
    uint8_t qtyLen;
};
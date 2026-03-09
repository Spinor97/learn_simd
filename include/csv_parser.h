#pragma once

#include <string_view>
#include <immintrin.h>
#include <cstdint>

struct blocks {
    u_int32_t comma_msk;
    u_int32_t nl_msk;
    u_int32_t quote_msk;
};

auto scan(const char* ptr) noexcept {
    __m256i chunk = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr));

    __m256i comma   = _mm256_set1_epi8(',');
    __m256i newline = _mm256_set1_epi8('\n');
    __m256i quote   = _mm256_set1_epi8('"');

    blocks rtn;

    rtn.comma_msk = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, comma));

    rtn.nl_msk = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, newline));

    rtn.quote_msk = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, quote));

    return rtn;
}

auto prexor(uint32_t msk, uint32_t &carry) {
    msk ^= carry;

    msk ^= msk << 1;
    msk ^= msk << 2;
    msk ^= msk << 4;
    msk ^= msk << 8;
    msk ^= msk << 16;

    carry = (msk >> 31) & 1 ? 0xFFFFFFFFu : 0;
    return msk;
}
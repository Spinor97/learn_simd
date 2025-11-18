#include <emmintrin.h>
#include <complex>
#include <vector>
#include <iostream>
using namespace std;

vector<int> get_pos_char(char const*data, size_t l, char tar) {
    vector<int> ret;
    ret.reserve(l);

    __m128i target = _mm_set1_epi8(tar);
    size_t curr = 0;

    while (curr + 16 <= l) {
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<__m128i const*>(data + curr));
        __m128i cmp = _mm_cmpeq_epi8(chunk, target);

        int mask = _mm_movemask_epi8(cmp);
        while (mask != 0) {
            int tz = __builtin_ctz(mask);
            ret.push_back(curr + tz);
            mask &= (mask - 1);
        }
        curr += 16;
    }

    while (curr < l) {
        if (data[curr] == tar) {
            ret.push_back(curr);
        }
        curr++;
    }

    return ret;
}


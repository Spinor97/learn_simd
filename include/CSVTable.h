#pragma once

#include <vector>
#include <string>

struct CSVTable {
    std::string buffer;
    std::vector<size_t> row;
    std::vector<size_t> col_stt;
    std::vector<size_t> col_end;

    size_t getRow() const {
        return row.size();
    }

    size_t getCol(size_t r) const {
        size_t stt = row[r];
        size_t ed = (r + 1 < getRow())
            ? row[r + 1]
            : col_stt.size();

        return ed - stt;
    }

    std::string_view get(size_t r, size_t c) noexcept {
        size_t idx = row[r] + c; 

        return std::string_view(
            buffer.data() + col_stt[idx],
            col_end[idx] - col_stt[idx]
        );
    }
};
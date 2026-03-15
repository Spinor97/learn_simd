#pragma once

#include <vector>
#include <string>
#include "col_type.h"
#include <memory>

struct CSVTable {
    std::string buffer;
    // std::vector<size_t> row;
    // std::vector<size_t> col_stt;
    // std::vector<size_t> col_end;

    std::vector<std::unique_ptr<ColType::IColumn>> columns;
    size_t row_cnt = 0;

    size_t getRow() const {
        return row_cnt;
    }

    size_t getCol(size_t r = 0) const {
        // size_t stt = row[r];
        // size_t ed = (r + 1 < getRow())
        //     ? row[r + 1]
        //     : col_stt.size();

        return columns.size();
    }

    // std::string_view get(size_t r, size_t c) noexcept {
    //     size_t idx = row[r] + c; 

    //     return std::string_view(
    //         buffer.data() + col_stt[idx],
    //         col_end[idx] - col_stt[idx]
    //     );
    // }
};
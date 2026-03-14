#pragma once

#include "csv_parser.h"
#include "CSVTable.h"
#include "col_type.h"
#include <memory>
#include <stdexcept>

using colType = ColType::Tp;

class CSVBuilder {
public:
    explicit CSVBuilder(std::string data, std::vector<colType> types)
        : table{}, schema_(std::move(types))
    {
        table.buffer = std::move(data);
        init_col();
    }

    CSVTable build() noexcept {
        parse();
        return std::move(table);
    }

private:
    CSVTable table;
    std::vector<colType> schema_;

    void init_col() {
        size_t expected_rows = table.buffer.length() / 32;
        table.columns.reserve(schema_.size());

        for (auto type : schema_) {
            switch (type) {
                case colType::Int64: {
                    auto col =
                        std::make_unique<ColType::intType>();
                    col->data.reserve(expected_rows);
                    table.columns.push_back(std::move(col));
                    break;
                }

                case colType::Double: {
                    auto col =
                        std::make_unique<ColType::doubleType>();
                    col->data.reserve(expected_rows);
                    table.columns.push_back(std::move(col));
                    break;
                }

                case colType::String: {
                    auto col =
                        std::make_unique<ColType::stringType>();
                    col->data.reserve(expected_rows);
                    table.columns.push_back(std::move(col));
                    break;
                }

                default:
                    throw std::runtime_error("Unknown col");
            }
        }
    }

    void parse() {
        const char* ptr = table.buffer.data();
        size_t len = table.buffer.size();

        uint32_t quote_carry = 0;
        size_t field_start = 0;
        size_t current_column = 0;

        size_t i = 0;

        while (i + 32 <= len) {
            auto masks = scan(ptr + i);

            uint32_t in_string =
                prexor(masks.quote_msk, quote_carry);

            uint32_t commas =
                masks.comma_msk & ~in_string;

            uint32_t newlines =
                masks.nl_msk & ~in_string;

            process_mask(commas, newlines,
                        i, field_start,
                        current_column);

            i += 32;
        }

        // tail
        for (; i < len; ++i) {
            char c = ptr[i];

            if (c == ',') {
                add_field(field_start, i, current_column);
                field_start = i + 1;
            }
            else if (c == '\n') {
                add_field(field_start, i, current_column);

                if (current_column != table.columns.size())
                    throw std::runtime_error("Column mismatch");

                current_column = 0;
                ++table.row_cnt;

                field_start = i + 1;
            }
        }

        if (field_start < len) {
            add_field(field_start, len, current_column);
        }
    }

    inline void process_mask(uint32_t commas, uint32_t newlines, size_t base,
                         size_t& field_start,
                         size_t& current_column) {
        while (commas) {
            int bit = __builtin_ctz(commas);
            size_t pos = base + bit;

            add_field(field_start, pos, current_column);
            field_start = pos + 1;

            commas &= commas - 1;
        }

        while (newlines) {
            int bit = __builtin_ctz(newlines);
            size_t pos = base + bit;

            add_field(field_start, pos, current_column);

            if (current_column != table.columns.size())
                throw std::runtime_error("Column mismatch");

            current_column = 0;
            ++table.row_cnt;

            field_start = pos + 1;
            newlines &= newlines - 1;
        }
    }

    inline void add_field(size_t start, size_t end, size_t& current_column) {
        const char* ptr = table.buffer.data() + start;
        size_t len = end - start;

        table.columns[current_column]->add(ptr, len);

        ++current_column;
    }
};
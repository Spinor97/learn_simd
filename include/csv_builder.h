#pragma once

#include "csv_parser.h"
#include "CSVTable.h"

class CSVBuilder {
public:
    explicit CSVBuilder(std::string data)
        : table{}
    {
        table.buffer = std::move(data);
    }

    CSVTable build() noexcept {
        parse();
        return std::move(table);
    }

private:
    CSVTable table;

    void parse() noexcept {
        const char* ptr = table.buffer.data();
        size_t len = table.buffer.size();

        uint32_t quote_carry = 0;
        size_t field_start = 0;

        table.row.push_back(0);

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
                         i, field_start);

            i += 32;
        }

        // tail
        for (; i < len; ++i) {
            char c = ptr[i];

            if (c == ',') {
                add_field(field_start, i);
                field_start = i + 1;
            }
            else if (c == '\n') {
                add_field(field_start, i);
                table.row.push_back(
                    table.col_stt.size());
                field_start = i + 1;
            }
        }

        if (field_start < len) {
            add_field(field_start, len);
        }
    }

    inline void process_mask(uint32_t commas,
                             uint32_t newlines,
                             size_t base,
                             size_t& field_start) {

        while (commas) {
            int bit = __builtin_ctz(commas);
            size_t pos = base + bit;
            add_field(field_start, pos);
            field_start = pos + 1;
            commas &= commas - 1;
        }

        while (newlines) {
            int bit = __builtin_ctz(newlines);
            size_t pos = base + bit;
            add_field(field_start, pos);
            table.row.push_back(
                table.col_stt.size());
            field_start = pos + 1;
            newlines &= newlines - 1;
        }
    }

    inline void add_field(size_t start, size_t end) {
        table.col_stt.push_back(start);
        table.col_end.push_back(end);
    }

    CSVTable table;
};
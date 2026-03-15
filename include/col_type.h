#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <charconv>
#include <string>

namespace ColType {
    enum class Tp {
        Int64,
        Double,
        String
    };

    struct IColumn {
        virtual ~IColumn() = default;
        virtual void add(const char* ptr, size_t len) noexcept = 0;
        virtual size_t size() const noexcept = 0;
    };

    template<typename T>
    struct baseType : IColumn {
        std::vector<T> data;
        std::vector<uint64_t> null;
        size_t cnt = 0;

        ~baseType() = default;

        void add(const char *ptr, size_t len) noexcept override {
            if (len == 0) {
                data.push_back(T{});
                push_null(true);
                return;
            }

            T ele;
            auto [p, ec] = std::from_chars(ptr, ptr + len, ele);
            if (ec != std::errc()) {
                data.push_back(T{});
                push_null(true);
            } else {
                data.push_back(ele);
                push_null(false);
            }
        };

        void push_null(bool isNull) noexcept {
            size_t idx = cnt >> 6, bit = cnt & 63;

            if (idx >= null.size()) {
                null.push_back(0);
            }

            if (isNull) {
                null[idx] |= 1ULL << bit;
            }

            ++cnt;
        }

        size_t size() const noexcept override {
            return data.size();
        }
    };
    
    template<>
    struct baseType<std::string_view>: IColumn{ // conditional code
        std::vector<std::string_view> data;
        std::vector<uint64_t> null;
        size_t cnt = 0;

        void add(const char *ptr, size_t len) noexcept override {
            if (len == 0) {
                data.emplace_back();
                push_null(true);
            } else {
                data.emplace_back(ptr, len);
                push_null(false);
            }
            
        }

        void push_null(bool isNull) noexcept {
            size_t idx = cnt >> 6, bit = cnt & 63;

            if (idx >= null.size()) {
                null.push_back(0);
            }

            if (isNull) {
                null[idx] |= 1ULL << bit;
            }

            ++cnt;
        }

        size_t size() const noexcept override {
            return data.size();
        }
    };
}

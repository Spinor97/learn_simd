#pragma once
#include <cstdint>
#include <cstddef>

struct baseType {
    virtual ~baseType();

    virtual void add(const char *ptr, size_t len) = 0;
    virtual size_t size() const = 0;
};

struct intType: baseType {

};

struct doubleType: baseType {

};

struct stringType: baseType {
    /* data */
};

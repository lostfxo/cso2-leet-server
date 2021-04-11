#ifndef __CSO2_VECTOR_H_
#define __CSO2_VECTOR_H_

#include <cstdint>

struct IntVector
{
    IntVector() : x(0), y(0), z(0) {}
    IntVector(int32_t _x, int32_t _y, int32_t _z) : x(_x), y(_y), z(_z) {}

    int32_t x;
    int32_t y;
    int32_t z;
};

#endif  // __CSO2_VECTOR_H_

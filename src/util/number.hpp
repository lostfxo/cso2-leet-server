#ifndef __UTIL_NUMBER_H_
#define __UTIL_NUMBER_H_

#include <limits>

namespace util
{
template <typename T, typename V>
T FastNarrow(V value)
{
    if (value < std::numeric_limits<T>::max())
    {
        return static_cast<T>(value);
    }
    else
    {
        return std::numeric_limits<T>::max() - 1;
    }
}
}  // namespace util

#endif  // __UTIL_NUMBER_H_

#ifndef __UTIL_JSON_CONV_H_
#define __UTIL_JSON_CONV_H_

#include <cstdlib>

#include <boost/json/string.hpp>

namespace json = boost::json;

inline std::uint64_t JStrToUint64(const json::string& str, int base = 10)
{
    return std::strtoul(str.data(), nullptr, base);
}

#endif  // __UTIL_JSON_CONV_H_

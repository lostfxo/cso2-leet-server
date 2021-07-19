#ifndef __UTIL_JSON_FWD_H_
#define __UTIL_JSON_FWD_H_

namespace boost
{
namespace json
{
#ifdef BOOST_JSON_STANDALONE
    inline namespace standalone
    {
#endif
        class value;
#ifdef BOOST_JSON_STANDALONE
    }
#endif
}  // namespace json
}  // namespace boost

#endif  // __UTIL_JSON_FWD_H_

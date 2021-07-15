#include "randomstring.hpp"

#include <array>
#include <random>

std::string GenerateRandomString(std::string::size_type length)
{
    constexpr const std::array<const char, 62> alphabet = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c',
        'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<std::size_t> dist(0,
                                                           alphabet.size() - 1);

    std::string result;
    result.reserve(length + 1);

    std::generate_n(std::back_inserter(result), length,
                    [&alphabet]() { return alphabet[dist(generator)]; });

    return result;
}

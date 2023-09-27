#pragma once

#include <iterator>

namespace Example::Util {

    template<typename T, typename TValue>
    concept InputRange = requires(T &&range) {
        requires std::input_iterator <std::ranges::iterator_t<decltype(range)>>;
        requires std::is_same_v<typename std::ranges::iterator_t<decltype(range)>::value_type, TValue>;
    };

}
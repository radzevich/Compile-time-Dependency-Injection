#include <iterator>

namespace Example::Util {

    template<typename T, typename TValue>
    concept InputRange = requires(T &&range) {
        std::input_iterator <std::ranges::iterator_t<decltype(range)>>;
        std::is_same_v<typename std::ranges::iterator_t<decltype(range)>::value_type, TValue>;
    };

}
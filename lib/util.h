#pragma once

#include <utility>

namespace IOC::Util {

    template <template<class ... TInnerArgs> class TOpenType>
    struct is_specialized {
    private:
        template<class ...TArgs, class TSwitcher = decltype(std::declval<TOpenType<TArgs...>>().~TOpenType<TArgs...>())>
        static constexpr bool Exists() {
            return true;
        }

        template<class ...TArgs>
        static constexpr bool Exists() {
            return false;
        }

    public:
//        static constexpr bool value = is_specialized<TOpenType, TSpecializationArgs...>::Exists<TSpecializationArgs...>;
        template<class ...TArgs>
        static constexpr bool Getvalue() {
            return Exists<TArgs...>();
        }
    };

    template <template<class ... TInnerArgs> class TOpenType, class ...TSpecializationArgs>
    using is_specialized_v = typename is_specialized<TOpenType>::template Getvalue<TSpecializationArgs...>();
}
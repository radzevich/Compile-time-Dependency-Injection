#pragma once

#include "../binding.h"

namespace IOC::Util {

    template <typename T>
    struct ReplaceDescriptors {
        using TResult = T;
    };

    template <template <typename ...> class T, typename ...TArgs>
    struct ReplaceDescriptors<T<TArgs...>> {
        using TResult = T<typename ReplaceDescriptors<typename Binding<TArgs>::TService>::TResult...>;
    };

    // TODO: add ReplaceDescriptors_t
}

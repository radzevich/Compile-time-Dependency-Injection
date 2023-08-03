#pragma once

#include "../binding.h"

namespace IOC::Util {

    template <typename T>
    struct EvaluateType {
        using Type = T;
    };

    template <template <typename ...> class T, typename ...TArgs>
    struct EvaluateType<T<TArgs...>> {
        using Type = T<typename EvaluateType<typename Binding<TArgs>::TService>::Type...>;
    };

    // TODO: add EvaluateType_t
}

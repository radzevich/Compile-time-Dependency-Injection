#pragma once

#include "service.h"
#include "../descriptors.h"
#include "../lib/ioc.h"

template <>
struct IOC::Binding<ADescriptor> {
    using TService = A;
};

template <>
struct IOC::Binding<BDescriptor> {
    using TService = B;
};

template <>
struct IOC::Binding<CDescriptor> {
    using TService = C;

    template <typename TServiceContainer>
    auto operator()(const TServiceContainer& container) {
        auto a = container.template Resolve<Singleton<ADescriptor>>();
        auto b = container.template Resolve<Scoped<BDescriptor>>();

        return C(a, b);
    }
};
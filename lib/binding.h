#pragma once

#include "lifetime.h"

namespace IOC {

    template <typename TDescriptor>
    struct Binding {
        using TLifetime = Transient;
        using TService = TDescriptor;
    };

}


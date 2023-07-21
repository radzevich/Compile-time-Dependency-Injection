#pragma once

#include "descriptors.h"
#include "src/booststrap.h"
#include "lib/ioc.h"

template class IOC::Container<
        ADescriptor,
        BDescriptor,
        CDescriptor>;

using TContainer = IOC::Container<
        ADescriptor,
        BDescriptor,
        CDescriptor>;
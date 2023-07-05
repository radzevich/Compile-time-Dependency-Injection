#include <iostream>
#include <optional>

#include "descriptors.h"
#include "lib/ioc.h"

using TServiceContainer = IOC::Container<
        ADescriptor,
        BDescriptor,
        CDescriptor>;

int main() {
    TServiceContainer ioc;

    auto a = ioc.Resolve<IOC::Singleton<ADescriptor>>();
    a->Whoami();

    auto b = ioc.Resolve<IOC::Scoped<BDescriptor>>();
    b->Whoami();

    auto c = ioc.Resolve<CDescriptor>();
    c.Whoami();

    return 0;
}

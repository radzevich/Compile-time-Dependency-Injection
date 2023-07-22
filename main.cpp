#include "lib/ioc_2.h"

void DoIoc2() {
    IOC2::ServiceCollection<
            ADescriptor,
            BDescriptor,
            CDescriptor> ioc;

    auto a = ioc.Resolve<ADescriptor>();
    a->Whoami();

    auto b = ioc.Resolve<BDescriptor>();
    b->Whoami();

    auto c = ioc.Resolve<CDescriptor>();
    c.Whoami();
}

int main() {
    DoIoc2();

    return 0;
}

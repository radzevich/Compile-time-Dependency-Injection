#include "bootstrap.h"
#include "descriptors.h"

int main() {
    TContainer ioc;

    auto a = ioc.Resolve<IOC::Singleton<ADescriptor>>();
    a->Whoami();

    auto b = ioc.Resolve<IOC::Scoped<BDescriptor>>();
    b->Whoami();

    auto c = ioc.Resolve<CDescriptor>();
    c.Whoami();

    return 0;
}

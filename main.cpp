#include "lib/ioc_2.h"

#include "examples/dao/bootstrap.h"

//void DoIoc2() {
//    auto container = IOC2::Container<ADescriptor, BDescriptor, CDescriptor>();
//
//    auto a = container.Resolve<ADescriptor>();
//    a->Whoami();
//
//    auto b = container.Resolve<BDescriptor>();
//    b->Whoami();
//
//    auto c = container.Resolve<CDescriptor>();
//    c.Whoami();
//}

int main() {
    TDaoContainer container;
    auto uow = container.Resolve<UnitOfWorkDescriptor>();

    return 0;
}

#include "lib/ioc_2.h"

#include "examples/dao/bootstrap.h"

using TContainer = class IOC2::ServiceCollection<Example::Dao::TContainer>;

int main() {
    TContainer container;
    auto uow = container.Resolve<Example::Dao::UnitOfWorkDescriptor>();
    auto companyId = uow->AddCompany("Good Company");
    std::cout << "WORKER: " << uow->AddEmployee(companyId, "Vasya") << std::endl;
    std::cout << "WORKER: " << uow->AddEmployee(companyId, "Petya") << std::endl;

    return 0;
}

#include "lib/ioc_2.h"

#include "examples/dao/bootstrap.h"

int main() {
    TDaoContainer container;
    auto uow = container.Resolve<UnitOfWorkDescriptor>();
    auto companyId = uow->AddCompany("Good Company");
    std::cout << "WORKER: " << uow->AddEmployee(companyId, "Vasya") << std::endl;
    std::cout << "WORKER: " << uow->AddEmployee(companyId, "Petya") << std::endl;

    return 0;
}

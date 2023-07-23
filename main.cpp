#include "lib/ioc.h"
#include "examples/dao/bootstrap.h"
#include <iostream>

using TContainer = class IOC::ServiceCollection<Example::Dao::TContainer>;

int main() {
    TContainer container;
    auto uow = container.Resolve<Example::Dao::UnitOfWorkDescriptor>();

    auto companyId = uow.AddCompany("Good Company");
    uow.AddEmployee(companyId, "Vasya");
    uow.AddEmployee(companyId, "Petya");

    for (auto employee : uow.GetCompanyEmployees(companyId)) {
        std::cout << "WORKER: " << employee.Id << " " << employee.Name << " works at " << employee.CompanyId << std::endl;
    }

    return 0;
}

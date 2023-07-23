#include "lib/ioc.h"
#include "examples/dao/bootstrap.h"
#include <iostream>

using TContainer = class IOC::ServiceCollection<Example::Dao::TContainer>;

int main() {
    TContainer container;
    auto uow = container.Resolve<Example::Dao::UnitOfWorkDescriptor>();

    auto departmentId = uow.AddDepartment("Good Department");
    uow.AddEmployee(departmentId, "Vasya");
    uow.AddEmployee(departmentId, "Petya");

    for (auto employee : uow.GetDepartmentEmployees(departmentId)) {
        std::cout << "WORKER: " << employee.Id << " " << employee.Name << " works at " << employee.DepartmentId << std::endl;
    }

    return 0;
}

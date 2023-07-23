#include "lib/ioc.h"
#include "examples/bootstrap.h"
#include "examples/exposure.h"
#include <iostream>

//using TContainer = IOC::ServiceCollection<Example::TContainer>;

int main() {
    IOC::ServiceCollection<Example::TContainer> container;
    auto uow = container.Resolve<Example::Services::UnitOfWorkDescriptor>();

    auto departmentId = uow.AddDepartment("Good Department");
    uow.AddEmployee(departmentId, "Vasya");
    uow.AddEmployee(departmentId, "Petya");

    for (auto employee : uow.GetDepartmentEmployees(departmentId)) {
        std::cout << "WORKER: " << employee.Id << " " << employee.Name << " works at " << employee.DepartmentId << std::endl;
    }

    return 0;
}

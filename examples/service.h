#pragma once

#include "bootstrap.h"

void DoJob() {

    auto developmentDepartmentId = 0;
    auto qaDepartmentId = 0;

    auto aliceId = 0;
    auto bobId = 0;

    {
        TServiceCollection services;
        auto departmentService = services.Resolve<Example::Services::Abstract::DepartmentManagementServiceDescriptor>();

        developmentDepartmentId = departmentService.Create("Development");
        qaDepartmentId = departmentService.Create("QA");
    }

    std::cout << std::endl;

    {
        TServiceCollection services;
        auto employeeService = services.Resolve<Example::Services::Abstract::EmployeeManagementServiceDescriptor>();

        bobId = employeeService.Hire(developmentDepartmentId, "Bob");
        aliceId = employeeService.Hire(qaDepartmentId, "Alice");
    }

    std::cout << std::endl;

    {
        TServiceCollection services;

        auto reportService = services.Resolve<Example::Services::Abstract::ReportManagementServiceDescriptor>();
        reportService.ListDepartments();

        auto employeeService = services.Resolve<Example::Services::Abstract::EmployeeManagementServiceDescriptor>();
        employeeService.Rotate(aliceId, qaDepartmentId, developmentDepartmentId);
        employeeService.Fire(developmentDepartmentId, bobId);

        // create ReportService once again
        reportService = services.Resolve<Example::Services::Abstract::ReportManagementServiceDescriptor>();
        reportService.ListDepartments();
    }

}

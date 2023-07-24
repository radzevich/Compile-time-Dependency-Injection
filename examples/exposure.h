#pragma once

#include "../lib/ioc.h"
#include "domain/employee.h"
#include "domain/department.h"

namespace Example {

    // infra
    struct LoggerDescriptor {};
    struct PrinterDescriptor {};

    // dao
    template <typename T>
    struct RepositoryDescriptor {};
    struct UnitOfWorkDescriptor {};

    // services
    struct DepartmentManagementServiceDescriptor {};
    struct EmployeeManagementServiceDescriptor {};
    struct ReportManagementServiceDescriptor {};

}

using TInfraServiceCollection = class IOC::ServiceCollection<
        Example::LoggerDescriptor,
        Example::PrinterDescriptor>;

using TDaoServiceCollection = class IOC::ServiceCollection<
        Example::RepositoryDescriptor<Example::Domain::Employee>,
        Example::RepositoryDescriptor<Example::Domain::Department>,
        Example::UnitOfWorkDescriptor>;

using TServiceServiceCollection = class IOC::ServiceCollection<
        Example::DepartmentManagementServiceDescriptor,
        Example::EmployeeManagementServiceDescriptor,
        Example::ReportManagementServiceDescriptor>;

using TServiceCollection = class IOC::ServiceCollection<
        TInfraServiceCollection,
        TDaoServiceCollection,
        TServiceServiceCollection>;

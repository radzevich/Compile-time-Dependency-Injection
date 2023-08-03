#pragma once

#include "exposure.h"
#include "domain/employee.h"
#include "domain/department.h"
#include "dao/repository.h"
#include "dao/unit_of_work.h"
#include "infra/logger.h"
#include "infra/printer.h"
#include "services/department_management.h"
#include "services/employee_management.h"
#include "services/report_management.h"

template<>
struct IOC::Binding<Example::Infra::Abstract::LoggerDescriptor> {
    using TLifetime = Scoped;
    using TService = Example::Infra::Logger;
};

template<>
struct IOC::Binding<Example::Infra::Abstract::PrinterDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Infra::Printer<>;
};

template<>
struct IOC::Binding<Example::Dao::Abstract::RepositoryDescriptor<Example::Domain::Employee>> {
    using TLifetime = Singleton;
    using TService = Example::Dao::Repository<Example::Domain::Employee>;
};

template<>
struct IOC::Binding<Example::Dao::Abstract::RepositoryDescriptor<Example::Domain::Department>> {
    using TLifetime = Singleton;
    using TService = Example::Dao::Repository<Example::Domain::Department>;
};

template<>
struct IOC::Binding<Example::Dao::Abstract::UnitOfWorkDescriptor> {
    using TLifetime = Scoped;
    using TService = Example::Dao::UnitOfWork<>;
};

template<>
struct IOC::Binding<Example::Services::Abstract::DepartmentManagementServiceDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Services::DepartmentManagementService<>;
};

template<>
struct IOC::Binding<Example::Services::Abstract::EmployeeManagementServiceDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Services::EmployeeManagementService<>;
};

template<>
struct IOC::Binding<Example::Services::Abstract::ReportManagementServiceDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Services::ReportManagementService<>;
};

template<>
struct IOC::ServiceFactory<Example::Infra::Logger> {
    static auto Create(const auto&) {
        return Example::Infra::Logger("[%s] %s %s\n");
    }
};

template<typename ...TArgs>
struct IOC::ServiceFactory<Example::Dao::Repository<TArgs...>> {
    static auto Create(const auto& container) {
        return Example::Dao::Repository<TArgs...>(container.template Resolve<Example::Infra::Abstract::LoggerDescriptor>());
    }
};

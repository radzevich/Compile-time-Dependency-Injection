#pragma once

#include "../lib/ioc.h"
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
struct IOC::Binding<Example::LoggerDescriptor> {
    using TLifetime = Scoped;
    using TService = Example::Infra::Logger;
};

template<>
struct IOC::Binding<Example::PrinterDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Infra::Printer<
            Binding<Example::LoggerDescriptor>::TService>;
};

template<>
struct IOC::Binding<Example::RepositoryDescriptor<Example::Domain::Employee>> {
    using TLifetime = Singleton;
    using TService = Example::Dao::Repository<Example::Domain::Employee>;
};

template<>
struct IOC::Binding<Example::RepositoryDescriptor<Example::Domain::Department>> {
    using TLifetime = Singleton;
    using TService = Example::Dao::Repository<Example::Domain::Department>;
};

template<>
struct IOC::Binding<Example::UnitOfWorkDescriptor> {
    using TLifetime = Scoped;
    using TService = Example::Dao::UnitOfWork<
            Binding<Example::RepositoryDescriptor<Example::Domain::Employee>>::TService,
            Binding<Example::RepositoryDescriptor<Example::Domain::Department>>::TService>;
};

template<>
struct IOC::Binding<Example::DepartmentManagementServiceDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Services::DepartmentManagementService<
            Binding<Example::UnitOfWorkDescriptor>::TService,
            Binding<Example::LoggerDescriptor>::TService>;
};

template<>
struct IOC::Binding<Example::EmployeeManagementServiceDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Services::EmployeeManagementService<
            Binding<Example::UnitOfWorkDescriptor>::TService,
            Binding<Example::LoggerDescriptor>::TService>;
};

template<>
struct IOC::Binding<Example::ReportManagementServiceDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Services::ReportManagementService<
            Binding<Example::UnitOfWorkDescriptor>::TService,
            Binding<Example::PrinterDescriptor>::TService,
            Binding<Example::LoggerDescriptor>::TService>;
};

template<>
struct IOC::ServiceFactory<Example::Infra::Logger> {
    static Example::Infra::Logger Create(const auto&) {
        return Example::Infra::Logger("[%s] %s %s\n");
    }
};

template<typename ...TArgs>
struct IOC::ServiceFactory<Example::Infra::Printer<TArgs...>> {
    static Example::Infra::Printer<TArgs...> Create(const auto& container) {
        return Example::Infra::Printer<TArgs...>(container.template Resolve<Example::LoggerDescriptor>());
    }
};

template<typename ...TArgs>
struct IOC::ServiceFactory<Example::Dao::UnitOfWork<TArgs...>> {
    static Example::Dao::UnitOfWork<TArgs...> Create(const auto& container) {
        return {
                container.template Resolve<Example::RepositoryDescriptor<Example::Domain::Employee>>(),
                container.template Resolve<Example::RepositoryDescriptor<Example::Domain::Department>>(),
        };
    }
};

template<typename ...TArgs>
struct IOC::ServiceFactory<Example::Dao::Repository<TArgs...>> {
    static Example::Dao::Repository<TArgs...> Create(const auto& container) {
        return Example::Dao::Repository<TArgs...>(container.template Resolve<Example::LoggerDescriptor>());
    }
};

template<typename ...TArgs>
struct IOC::ServiceFactory<Example::Services::DepartmentManagementService<TArgs...>> {
    static Example::Services::DepartmentManagementService<TArgs...> Create(const auto& container) {
        return {
                container.template Resolve<Example::UnitOfWorkDescriptor>(),
                container.template Resolve<Example::LoggerDescriptor>(),
        };
    }
};

template<typename ...TArgs>
struct IOC::ServiceFactory<Example::Services::EmployeeManagementService<TArgs...>> {
    static Example::Services::EmployeeManagementService<TArgs...> Create(const auto& container) {
        return {
                container.template Resolve<Example::UnitOfWorkDescriptor>(),
                container.template Resolve<Example::LoggerDescriptor>(),
        };
    }
};

template<typename ...TArgs>
struct IOC::ServiceFactory<Example::Services::ReportManagementService<TArgs...>> {
    static Example::Services::ReportManagementService<TArgs...> Create(const auto& container) {
        return {
                container.template Resolve<Example::UnitOfWorkDescriptor>(),
                container.template Resolve<Example::PrinterDescriptor>(),
                container.template Resolve<Example::LoggerDescriptor>(),
        };
    }
};

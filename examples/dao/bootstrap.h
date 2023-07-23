#pragma once

#include "../../lib/ioc.h"
#include "exposure.h"
#include "repository.h"
#include "unit_of_work.h"

template<>
struct IOC::Binding<Example::Dao::EmployeeRepositoryDescriptor> {
    using TLifetime = Singleton;
    using TService = Example::Dao::Repository<Example::Domain::Employee>;
};

template<>
struct IOC::Binding<Example::Dao::DepartmentRepositoryDescriptor> {
    using TLifetime = Singleton;
    using TService = Example::Dao::Repository<Example::Domain::Department>;
};

template<>
struct IOC::Binding<Example::Dao::UnitOfWorkDescriptor> {
    using TLifetime = Transient;
    using TService = Example::Dao::UnitOfWork<
        IOC::Binding<Example::Dao::EmployeeRepositoryDescriptor>::TService,
        IOC::Binding<Example::Dao::DepartmentRepositoryDescriptor>::TService>;
};

template<>
struct IOC::ServiceFactory<IOC::Binding<Example::Dao::UnitOfWorkDescriptor>::TService> {
    static IOC::Binding<Example::Dao::UnitOfWorkDescriptor>::TService Create(const auto& container) {
        return {
            container.template Resolve<Example::Dao::EmployeeRepositoryDescriptor>(),
            container.template Resolve<Example::Dao::DepartmentRepositoryDescriptor>()
        };
    }
};

namespace Example::Dao {

    using TContainer = class IOC::ServiceCollection<
        EmployeeRepositoryDescriptor,
        DepartmentRepositoryDescriptor,
        UnitOfWorkDescriptor>;

}
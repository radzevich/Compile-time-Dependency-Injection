#pragma once

#include "../../lib/ioc_2.h"
#include "dependency_exposure.h"
#include "repository.h"
#include "unit_of_work.h"

template<>
struct IOC2::Binding<Example::Dao::EmployeeRepositoryDescriptor> {
    using TLifetime = Scoped;
    using TService = Example::Dao::Repository<Employee>;
};

template<>
struct IOC2::Binding<Example::Dao::CompanyRepositoryDescriptor> {
    using TLifetime = Scoped;
    using TService = Example::Dao::Repository<Company>;
};

template<>
struct IOC2::Binding<Example::Dao::UnitOfWorkDescriptor> {
    using TLifetime = Scoped;
    using TService = Example::Dao::UnitOfWork<
            IOC2::Binding<Example::Dao::EmployeeRepositoryDescriptor>::TService,
            IOC2::Binding<Example::Dao::CompanyRepositoryDescriptor>::TService>;
};

namespace Example::Dao {

    using TContainer = class IOC2::ServiceCollection<
            EmployeeRepositoryDescriptor,
            CompanyRepositoryDescriptor,
            UnitOfWorkDescriptor>;

}
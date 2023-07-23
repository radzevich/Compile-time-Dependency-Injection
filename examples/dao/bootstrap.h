#pragma once

#include "../../lib/ioc_2.h"
#include "dependency_exposure.h"
#include "repository.h"

template<>
struct IOC2::Binding<EmployeeRepositoryDescriptor> {
    using TLifetime = Scoped;
    using TService = Repository<Employee>;
};

template<>
struct IOC2::Binding<CompanyRepositoryDescriptor> {
    using TLifetime = Scoped;
    using TService = Repository<Company>;
};

template<>
struct IOC2::Binding<UnitOfWorkDescriptor> {
    using TLifetime = Scoped;
    using TService = UnitOfWork<
            IOC2::Binding<EmployeeRepositoryDescriptor>::TService,
            IOC2::Binding<CompanyRepositoryDescriptor>::TService>;
};

using TDaoContainer = class IOC2::ServiceCollection<
        EmployeeRepositoryDescriptor,
        CompanyRepositoryDescriptor,
        UnitOfWorkDescriptor>;
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

template <typename ...TRepositories>
struct IOC2::ServiceFactory<UnitOfWork<TRepositories...>> {
    template <typename TContainer>
    static UnitOfWork<TRepositories...> Create(const TContainer& container) {
        auto* employeeRepository = container.template Resolve<EmployeeRepositoryDescriptor>();
        auto* companyRepository = container.template Resolve<CompanyRepositoryDescriptor>();

        return UnitOfWork(employeeRepository, companyRepository);
    }
};

template <>
struct IOC2::ServiceFactory<Repository<Employee>> {
    template <typename TContainer>
    static Repository<Employee> Create(const TContainer&) {
        return {};
    }
};

template <>
struct IOC2::ServiceFactory<Repository<Company>> {
    template <typename TContainer>
    static Repository<Employee> Create(const TContainer&) {
        return {};
    }
};

//template class IOC2::ServiceCollection<
//        EmployeeRepositoryDescriptor,
//        CompanyRepositoryDescriptor,
//        UnitOfWorkDescriptor>;

using TDaoContainer = class IOC2::ServiceCollection<
        EmployeeRepositoryDescriptor,
        CompanyRepositoryDescriptor,
        UnitOfWorkDescriptor>;
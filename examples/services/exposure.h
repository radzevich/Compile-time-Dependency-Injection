#pragma once

#include "../../lib/ioc_2.h"
#include "service.h"

struct EmployeeRepositoryDescriptor {};

struct CompanyRepositoryDescriptor {};

//template <>
//struct IOC2::ServiceFactory<AccountService> {
//    template <typename TContainer>
//    static AccountService Create(const TContainer& container) {
//        auto* employeeRepository = container.template Resolve<EmployeeRepositoryDescriptor>();
//        auto* companyRepository = container.template Resolve<CompanyRepositoryDescriptor>();
//
//        return AccountService(employeeRepository, companyRepository);
//    }
//};

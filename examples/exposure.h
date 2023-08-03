#pragma once

#include "domain/employee.h"
#include "domain/department.h"
#include "../lib/service_collection.h"
#include "infra/abstract/logger_descriptor.h"
#include "infra/abstract/printer_descriptor.h"
#include "dao/abstract/repository_descriptor.h"
#include "dao/abstract/unit_of_work_descriptor.h"
#include "services/abstract/department_management_descriptor.h"
#include "services/abstract/employee_management_descriptor.h"
#include "services/abstract/report_management_descriptor.h"

using TInfraServiceCollection = class IOC::ServiceCollection<
        Example::Infra::Abstract::LoggerDescriptor,
        Example::Infra::Abstract::PrinterDescriptor>;

using TDaoServiceCollection = class IOC::ServiceCollection<
        Example::Dao::Abstract::RepositoryDescriptor<Example::Domain::Employee>,
        Example::Dao::Abstract::RepositoryDescriptor<Example::Domain::Department>,
        Example::Dao::Abstract::UnitOfWorkDescriptor>;

using TServiceServiceCollection = class IOC::ServiceCollection<
        Example::Services::Abstract::DepartmentManagementServiceDescriptor,
        Example::Services::Abstract::EmployeeManagementServiceDescriptor,
        Example::Services::Abstract::ReportManagementServiceDescriptor>;

using TServiceCollection = class IOC::ServiceCollection<
        TInfraServiceCollection,
        TDaoServiceCollection,
        TServiceServiceCollection>;

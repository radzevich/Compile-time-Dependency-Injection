#pragma once

#include "../domain//department.h"
#include "../domain/employee.h"
#include "../utils/enumerable.h"
#include "../utils/input_range.h"

#include <concepts>
#include <string>

namespace Example::Services {

    template<class TUnitOfWork>
    concept UnitOfWork = requires(TUnitOfWork& uow, int departmentId) {
        { uow.GetDepartments() } -> Util::InputRange<Domain::Department>;
        { uow.GetDepartmentEmployees(departmentId) } -> Util::InputRange<Domain::Employee>;
    };

    template<class TPrinter>
    concept Printer = requires(TPrinter& printer) {
        requires requires(TPrinter& p, const Domain::Employee& employee) {
            p << employee;
        };

        requires requires(TPrinter& p, const Domain::Department& department) {
            p << department;
        };
    };

    template<class TLogger>
    concept Logger = requires(TLogger& logger, const char* message) {
        { logger.Info(message) };
    };

    template<UnitOfWork TUnitOfWork, Printer TPrinter, Logger TLogger>
    class ReportManagementService {
    private:
        TUnitOfWork* UnitOfWork_;
        TPrinter Printer_;
        TLogger* Logger_;

    public:
        explicit ReportManagementService(TUnitOfWork* unitOfWork, TPrinter printer, TLogger* logger);

        void ListDepartments() const;
        void ListEmployees(int departmentId) const;
    };

}

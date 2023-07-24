#pragma once

#include "../domain//department.h"
#include "../domain/employee.h"
#include "../utils/enumerable.h"
#include "../utils/input_range.h"

#include <concepts>
#include <string>

namespace Example::Services {

    template<typename TUnitOfWork, typename TPrinter, typename TLogger>
    class ReportManagementService {
    private:
        TUnitOfWork* UnitOfWork_;
        mutable TPrinter Printer_;
        TLogger* Logger_;

    public:
        ReportManagementService(TUnitOfWork* unitOfWork, TPrinter printer, TLogger* logger);

        void ListDepartments() const;
        void ListEmployees(int departmentId) const;
    };

    template<typename TUnitOfWork, typename TPrinter, typename TLogger>
    ReportManagementService<TUnitOfWork, TPrinter, TLogger>::ReportManagementService(TUnitOfWork* unitOfWork,
                                                                                     TPrinter printer,
                                                                                     TLogger* logger)
        : UnitOfWork_(unitOfWork)
        , Printer_(std::move(printer))
        , Logger_(logger) {
        Logger_->Info("Created ReportManagementService");
    }

    template<typename TUnitOfWork, typename TPrinter, typename TLogger>
    void ReportManagementService<TUnitOfWork, TPrinter, TLogger>::ListEmployees(int departmentId) const {
        for (const auto& employee : UnitOfWork_->GetDepartmentEmployees(departmentId)) {
            Printer_ << employee;
        }
    }

    template<typename TUnitOfWork, typename TPrinter, typename TLogger>
    void ReportManagementService<TUnitOfWork, TPrinter, TLogger>::ListDepartments() const {
        for (const auto& department : UnitOfWork_->GetDepartments()) {
            Printer_ << department;
        }
    }
}

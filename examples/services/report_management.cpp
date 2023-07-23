#include "report_management.h"

#include <iostream>

namespace Example::Services {

    template<UnitOfWork TUnitOfWork, Printer TPrinter, Logger TLogger>
    ReportManagementService<TUnitOfWork, TPrinter, TLogger>::ReportManagementService(TUnitOfWork* unitOfWork,
                                                                                     TPrinter printer,
                                                                                     TLogger* logger)
        : UnitOfWork_(unitOfWork)
        , Printer_(std::move(printer))
        , Logger_(logger) {
    }

    template<UnitOfWork TUnitOfWork, Printer TPrinter, Logger TLogger>
    void ReportManagementService<TUnitOfWork, TPrinter, TLogger>::ListEmployees(int departmentId) const {
        Logger_->Info("ListEmployees");

        for (const auto& employee : UnitOfWork_->GetDepartmentEmployees(departmentId)) {
            Printer_ << employee << std::endl;
        }
    }

    template<UnitOfWork TUnitOfWork, Printer TPrinter, Logger TLogger>
    void ReportManagementService<TUnitOfWork, TPrinter, TLogger>::ListDepartments() const {
        Logger_->Info("ListDepartments");

        for (const auto& department : UnitOfWork_->GetDepartments) {
            Printer_ << department << std::endl;
        }
    }

}
#pragma once

#include "../domain//department.h"
#include "../domain/employee.h"
#include "../utils/enumerable.h"
#include "../utils/input_range.h"
#include "exposure.h"

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

template<typename ...TDeps>
struct IOC::ServiceFactory<Example::Services::ReportManagementService<TDeps...>> {
    static Example::Services::ReportManagementService<TDeps...> Create(const auto& container) {
        return {
            container.template Resolve<Example::Services::UnitOfWorkDescriptor>(),
            container.template Resolve<Example::Services::PrinterDescriptor>(),
            container.template Resolve<Example::Services::LoggerDescriptor>()
        };
    }
};

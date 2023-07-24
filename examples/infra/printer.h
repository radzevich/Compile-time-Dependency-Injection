#pragma once

#include "../domain/employee.h"
#include "../domain/department.h"
#include <iostream>
#include <sstream>

namespace Example::Infra {

    template <typename TLogger>
    class Printer {
    private:
        TLogger* Logger_;

    public:
        explicit Printer(TLogger* logger) : Logger_(logger) {
        };

        friend Printer<TLogger>& operator<<(Printer<TLogger>& printer, const Domain::Employee& employee) {
            printer.Logger_->Info(std::stringstream() << "Employee " << employee.Name
                                                << " with id " << employee.Id
                                                << " works at " << employee.DepartmentId
                                                << std::endl);

            return printer;
        }

        friend Printer<TLogger>& operator<<(Printer<TLogger>& printer, const Domain::Department& department) {
            std::stringstream ss;
            ss  << "Department " << department.Name
                << " with id " << department.Id
                << " has employees: ";

            for (int employeeId : department.Employees) {
                ss << employeeId << ' ';
            }

            printer.Logger_->Info(ss.str());

            return printer;
        }
    };

}

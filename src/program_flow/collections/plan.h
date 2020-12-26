#ifndef BANKMANAGEMENT_PLAN_H
#define BANKMANAGEMENT_PLAN_H

#include <string>
#include <filesystem>
#include "../../db/db_management.h"
#include "../../structures/output_logger_manager.h"
#include "../account_management_actions/plan_management.h"

class plan {
public:
    plan(db_management *db_ptr, const std::string &account_name, const std::string &plan_name, output_logger_manager *output);
    ~plan();

    double delete_plan();

    [[nodiscard]] std::string get_plan_name() const;
    [[nodiscard]] double get_plan_cash() const;
    void print_details() const;
    void actions();

    bool operator==(const std::string &name) const;
    bool operator!=(const std::string &name) const;

private:
    db_management *db_ptr;
    std::string account_name;
    std::string plan_name;
    plan_management management;
    output_logger_manager *output;
};

#endif //BANKMANAGEMENT_PLAN_H
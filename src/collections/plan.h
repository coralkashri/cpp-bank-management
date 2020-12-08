#ifndef BANKMANAGEMENT_PLAN_H
#define BANKMANAGEMENT_PLAN_H

#include <string>
#include <filesystem>
#include "../db/db_management.h"
#include "../structures/output_logger_manager.h"

class plan {
public:
    plan(db_management *db_ptr, const std::string &account_name, const std::string &plan_name, output_logger_manager *output);
    ~plan();

    bool delete_plan();

    std::string get_plan_name() const;
    double get_plan_cash();
    void print_details() const;
    void plan_management();

    bool operator==(const std::string &name);
    bool operator!=(const std::string &name);

private:
    db_management *db_ptr;
    std::string account_name;
    std::string plan_name;
    double cash;
    output_logger_manager *output;
};

#endif //BANKMANAGEMENT_PLAN_H
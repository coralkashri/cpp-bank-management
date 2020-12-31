#include "plan.h"
#include "../../db/db_management.h"
#include "../../extensions/custom_exceptions.h"
#include <iostream>
#include <filesystem>
#include <fstream>

plan::plan(db_management *db_ptr, const std::string &account_name, const std::string &plan_name, output_logger_manager *output)
        : db_ptr(db_ptr), account_name(account_name), plan_name(plan_name), output(output),
        management(db_ptr, account_name, plan_name, output) {
    try {
        db_ptr->create_plan(account_name, plan_name);
    } catch (plan_already_exists_exception &e) {
        // Sync plan with db
        this->output->printer() << "Plan " << plan_name << " already exists in this account. Details:\n";
        print_details();
    }
}

plan::~plan() {

}

std::string plan::get_plan_name() const {
    return plan_name;
}

double plan::get_plan_cash() const {
    return db_ptr->get_plan_balance(account_name, plan_name);
}

bool plan::operator==(const std::string &name) const {
    return plan_name == name;
}

bool plan::operator!=(const std::string &name) const {
    return !(*this == name);
}

void plan::print_details() const {
    management.print_details();
}

void plan::actions() {
    management.actions();
}

double plan::delete_plan() {
    // Remove plan from db and return it's cash
    return db_ptr->delete_plan(account_name, plan_name);
}
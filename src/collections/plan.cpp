#include "plan.h"
#include "../db/db_management.h"
#include "../extensions/custom_exceptions.h"
#include <iostream>
#include <filesystem>
#include <fstream>

plan::plan(db_management *db_ptr, const std::string &account_name, const std::string &plan_name)
        : db_ptr(db_ptr), account_name(account_name), plan_name(plan_name) {
    try {
        db_ptr->create_plan(account_name, plan_name);
        cash = 0;
    } catch (plan_already_exists_exception &e) {
        // Sync plan with db
        cash = db_ptr->get_plan_balance(account_name, plan_name);
        std::cout << "Plan " << plan_name << " already exists in this account. Details:\n";
        print_details();
    }
}

plan::~plan() {

}

std::string plan::get_plan_name() const {
    return plan_name;
}

double plan::get_plan_cash() {
    return cash;
}

bool plan::operator==(const std::string &name) {
    return plan_name == name;
}

bool plan::operator!=(const std::string &name) {
    return !(*this == name);
}

void plan::print_details() const {
    std::cout << "====================================\n";
    std::cout << "Account: " << account_name << "\n";
    std::cout << "Plan: " << plan_name << "\n";
    std::cout << "Cash: " << cash << "\n";
    std::cout << "====================================\n";
}

void plan::plan_management() {
    // todo plan_management object
}

bool plan::delete_plan() {
    db_ptr->delete_plan(account_name, plan_name);
    return true;
}
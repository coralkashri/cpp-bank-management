#include <fstream>
#include "account.h"
#include "../extensions/custom_exceptions.h"

account::account(const std::string &db_path, const std::string &account_name) : db_path(db_path), account_name(account_name), available_cash(0) {
    // Create account in db
    if (!std::filesystem::is_directory(dir_path()))
        std::filesystem::create_directory(dir_path());

    // Set current cash state to 0
    if (auto file_path = (dir_path()/account_name) += "_cash.bin"; !std::filesystem::is_regular_file(file_path)) {
        std::ofstream new_plan_file(file_path.string(), std::ios::trunc);
        new_plan_file << 0;
    }
}

account::~account() {
}

bool account::delete_account() {
    // Remove account from db
    return std::filesystem::is_directory(dir_path()) && std::filesystem::remove(dir_path());
}

std::string account::get_account_name() const {
    return account_name;
}

std::filesystem::path account::dir_path() {
    return std::filesystem::path(db_path)/account_name;
}

bool account::operator==(const std::string &name) {
    return name == account_name;
}

std::vector<std::string> account::get_plan_names() const {
    std::vector<std::string> plan_names;
    for (auto &plan : plans) {
        plan_names.emplace_back(plan.get_plan_name());
    }
    return plan_names;
}

bool account::operator!=(const std::string &name) {
    return !(*this == name);
}

void account::create_plan(const std::string &plan_name) {
    try {
        find_plan(plan_name);
        throw plan_already_exists_exception();
    } catch (plan_not_found_exception &e) {
        plans.emplace_back(dir_path(), plan_name);
    }
}

void account::print_plan_details(const std::string &plan_name) {
    auto plan_it = find_plan(plan_name);
    plan_it->print_details();
}

void account::plan_management(const std::string &plan_name) {
    auto plan_it = find_plan(plan_name);
    plan_it->plan_management();
}

void account::remove_plan(std::string plan_name) {
    auto plan_it = find_plan(plan_name);
    if (plan_it->delete_plan()) {
        plans.erase(plan_it);
    } else throw std::runtime_error("Can't delete plan: " + plan_name);
}

std::vector<plan>::iterator account::find_plan(const std::string &name) {
    if (auto it = std::find(plans.begin(), plans.end(), name); it != plans.end()) {
        return it;
    }
    throw plan_not_found_exception();
}
#include "available_user_actions.h"
#include <iostream>
#include "../extensions/custom_exceptions.h"
#include "../extensions/std_extensions.h"

void available_user_actions::create_account() {
    std::string account_name;
    input("account name", account_name);
    bank_obj.create_account(account_name);
    std::cout << "Account " << account_name << " successfully created.\n";
}

void available_user_actions::account_login() {
    account_logout_validation();
    std::string desired_account;
    input("account name", desired_account);
    current_account = &bank_obj.get_account(desired_account);
    std::cout << "Successful login.\n";
}

void available_user_actions::get_available_accounts() {
    account_logout_validation();
    std::cout << bank_obj.get_account_names();
}

void available_user_actions::get_available_plans() {
    account_login_validation();
    std::cout << current_account->get_plan_names();
}

void available_user_actions::create_plan() {
    account_login_validation();
    std::string plan_name;
    input("plan name", plan_name);
    current_account->create_plan(plan_name);
    std::cout << "Plan \"" + plan_name + "\" successful created.\n";
}

void available_user_actions::plan_management() {
    account_login_validation();
    std::string plan_name;
    input("plan name", plan_name);
    current_account->plan_management(plan_name);
}

void available_user_actions::account_logout() {
    account_login_validation();
    current_account = nullptr;
    std::cout << "Successfully logout.\n";
}

void available_user_actions::remove_account() {
    account_logout_validation();
    std::string desired_account;
    input("account name", desired_account);
    bank_obj.delete_account(desired_account);
    std::cout << "Account \"" << desired_account << "\" successfully deleted.\n";
}

void available_user_actions::remove_plan() {
    account_login_validation();
    std::string plan_name;
    input("plan name", plan_name);
    current_account->remove_plan(plan_name);
    std::cout << "Plan \"" << plan_name << "\" successfully deleted.\n";
}


bool available_user_actions::is_logged_in() {
    return current_account;
}

void available_user_actions::account_login_validation() {
    if (!is_logged_in())
        throw account_not_selected_exception();
}

void available_user_actions::account_logout_validation() {
    if (is_logged_in())
        throw logout_requirement_exception();
}

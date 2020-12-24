#include "db_management.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include "../extensions/custom_exceptions.h"

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

db_management::db_management() : client(mongocxx::uri{}),
                                db(client["bank_management_db"]),
                                plans_management(&db),
                                accounts_management(&db) {
}

// Accounts Management

void db_management::create_account(const std::string &account_name) {
    accounts_management.create_account(account_name);
}

void db_management::delete_account(const std::string &account_name) {
    plans_management.delete_all_account_plans(account_name);
    accounts_management.delete_account(account_name);
}

void db_management::modify_account_free_cash(const std::string &account_name, double cash) {
    accounts_management.modify_free_cash(account_name, cash);
}

double db_management::get_account_free_cash(const std::string &account_name) const {
    return accounts_management.get_account_free_cash(account_name);
}


// Account Transactions Management

void db_management::update_account_monthly_income(const std::string &account_name, const std::string &source_name, double income) {
    accounts_management.get_transactions_management().update_account_monthly_income(account_name, source_name, income);
}

void db_management::set_account_single_time_income(const std::string &account_name, const std::string &source_name, double income) {
    accounts_management.get_transactions_management().set_account_single_time_income(account_name, source_name, income);
}

void db_management::pause_account_monthly_income(const std::string &account_name, const std::string &source_name) {
    accounts_management.get_transactions_management().pause_account_monthly_income(account_name, source_name);

}

void db_management::restart_account_monthly_income(const std::string &account_name, const std::string &source_name) {
    accounts_management.get_transactions_management().restart_account_monthly_income(account_name, source_name);
}

void db_management::update_account_monthly_outcome(const std::string &account_name, const std::string &target_name, double outcome) {
    accounts_management.get_transactions_management().update_account_monthly_outcome(account_name, target_name, outcome);
}

void db_management::set_account_single_time_outcome(const std::string &account_name, const std::string &target_name, double outcome) {
    accounts_management.get_transactions_management().set_account_single_time_outcome(account_name, target_name, outcome);
}

void db_management::pause_account_monthly_outcome(const std::string &account_name, const std::string &target_name) {

}

void db_management::restart_account_monthly_outcome(const std::string &account_name, const std::string &target_name) {

}


// Plans Management

void db_management::create_plan(const std::string &account_name, const std::string &plan_name) {
    if (!is_account_exists(account_name)) throw account_not_found_exception();
    plans_management.create_plan(account_name, plan_name);
}

double db_management::delete_plan(const std::string &account_name, const std::string &plan_name) {
    if (!is_account_exists(account_name)) throw account_not_found_exception();
    return plans_management.delete_plan(account_name, plan_name);
}

void db_management::modify_plan_balance(const std::string &account_name, const std::string &plan_name, double cash) {
    if (!is_account_exists(account_name)) throw account_not_found_exception();
    plans_management.modify_plan_balance(account_name, plan_name, cash);
}

double db_management::get_plan_balance(const std::string &account_name, const std::string &plan_name) const {
    if (!is_account_exists(account_name)) throw account_not_found_exception();
    return plans_management.get_plan_balance(account_name, plan_name);
}

std::vector<std::string> db_management::get_all_accounts() const {
    return accounts_management.get_all_accounts();
}

std::vector<std::string> db_management::get_all_account_plans(const std::string &account_name) const {
    if (!is_account_exists(account_name)) throw account_not_found_exception();
    return plans_management.get_all_account_plans(account_name);
}

bool db_management::is_account_exists(const std::string &account_name) const {
    return accounts_management.is_account_exists(account_name);
}
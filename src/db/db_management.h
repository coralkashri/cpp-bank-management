#ifndef BANKMANAGEMENT_DB_MANAGEMENT_H
#define BANKMANAGEMENT_DB_MANAGEMENT_H

#include <string>
#include <vector>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include "plans_db_management.h"
#include "accounts_db_management.h"

class db_management {
public:
    db_management();

    void create_account(const std::string &account_name);
    void delete_account(const std::string &account_name);
    double get_account_free_cash(const std::string &account_name);
    void create_plan(const std::string &account_name, const std::string &plan_name);
    void delete_plan(const std::string &account_name, const std::string &plan_name);
    void modify_plan_balance(const std::string &account_name, const std::string &plan_name, double cash);
    double get_plan_balance(const std::string &account_name, const std::string &plan_name);
    std::vector<std::string> get_all_accounts();
    std::vector<std::string> get_all_account_plans(const std::string &account_name);

private:
    mongocxx::instance instance;
    mongocxx::client client;
    mongocxx::database db;
    plans_db_management plans_management;
    accounts_db_management accounts_management;

    bool is_account_exists(const std::string &account_name);
    void delete_all_account_plans(const std::string &account_name);
};

#endif //BANKMANAGEMENT_DB_MANAGEMENT_H
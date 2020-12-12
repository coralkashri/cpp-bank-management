#ifndef BANKMANAGEMENT_PLANS_DB_MANAGEMENT_H
#define BANKMANAGEMENT_PLANS_DB_MANAGEMENT_H

#include <string>
#include <vector>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>

class plans_db_management {
public:
    explicit plans_db_management(mongocxx::database *db_ptr);

    void create_plan(const std::string &account_name, const std::string &plan_name);
    void delete_plan(const std::string &account_name, const std::string &plan_name);
    void modify_plan_balance(const std::string &account_name, const std::string &plan_name, double cash);
    [[nodiscard]] double get_plan_balance(const std::string &account_name, const std::string &plan_name) const;
    [[nodiscard]] std::vector<std::string> get_all_account_plans(const std::string &account_name) const;
    void delete_all_account_plans(const std::string &account_name);

private:
    std::string plans_table_name;
    mongocxx::database *db_ptr;
    [[nodiscard]] bool is_plan_exists(const std::string &account_name, const std::string &plan_name) const;
};

#endif //BANKMANAGEMENT_PLANS_DB_MANAGEMENT_H
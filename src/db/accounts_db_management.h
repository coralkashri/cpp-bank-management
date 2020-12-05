#ifndef BANKMANAGEMENT_ACCOUNTS_DB_MANAGEMENT_H
#define BANKMANAGEMENT_ACCOUNTS_DB_MANAGEMENT_H

#include <string>
#include <vector>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>

class accounts_db_management {
public:
    explicit accounts_db_management(mongocxx::database *db_ptr);

    void create_account(const std::string &account_name);
    void delete_account(const std::string &account_name);
    std::vector<std::string> get_all_accounts();
    bool is_account_exists(const std::string &account_name);
    double get_account_free_cash(const std::string &account_name);

private:
    std::string accounts_table_name;
    mongocxx::database *db_ptr;
};

#endif //BANKMANAGEMENT_ACCOUNTS_DB_MANAGEMENT_H
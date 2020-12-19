#ifndef BANKMANAGEMENT_ACCOUNTS_DB_MANAGEMENT_H
#define BANKMANAGEMENT_ACCOUNTS_DB_MANAGEMENT_H

#include <string>
#include <vector>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <boost/date_time.hpp>
#include "../program_flow/structures/income_source.h"

class accounts_db_management {
public:
    explicit accounts_db_management(mongocxx::database *db_ptr);

    void create_account(const std::string &account_name);
    void delete_account(const std::string &account_name);
    void modify_free_cash(const std::string &account_name, double cash);

    /// Will store the income details for the current month
    void update_account_monthly_income(const std::string &account_name, const std::string &income_source_name, double income);

    void pause_account_monthly_income(const std::string &account_name, const std::string &income_source);

    /// Will store the income details for the current month
    void set_account_single_time_income(const std::string &account_name, double income);

    /// Will store the outcome details for the current month
    void update_account_monthly_outcome(const std::string &account_name, const std::string &outcome_name, double outcome_cash);

    /// Will store the outcome details for the current month
    void add_account_single_time_outcome(const std::string &account_name, const std::string &outcome_name, double outcome_cash);

    [[nodiscard]] std::vector<outcome_source> get_account_outcome_details(const std::string &account_name, const boost::gregorian::date &month) const;
    [[nodiscard]] std::vector<income_source> get_account_income_details(const std::string &account_name, const boost::gregorian::date &month) const;
    [[nodiscard]] std::vector<std::string> get_all_accounts() const;
    [[nodiscard]] bool is_account_exists(const std::string &account_name) const;
    [[nodiscard]] double get_account_free_cash(const std::string &account_name) const;

private:
    std::string accounts_table_name;
    mongocxx::database *db_ptr;
};

#endif //BANKMANAGEMENT_ACCOUNTS_DB_MANAGEMENT_H
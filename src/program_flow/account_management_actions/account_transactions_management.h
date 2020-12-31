#ifndef BANKMANAGEMENT_ACCOUNT_TRANSACTIONS_MANAGEMENT_H
#define BANKMANAGEMENT_ACCOUNT_TRANSACTIONS_MANAGEMENT_H

#include <string>
#include "../../db/db_management.h"
#include "../../structures/output_logger_manager.h"
#include "../../external_tools/multimap/multimap.h"
#include "account_management_actions_interface.h"

class account_transactions_management : public account_management_actions_interface<account_transactions_management> {
public:
    friend account_management_actions_interface<account_transactions_management>;

    account_transactions_management(db_management *db_ptr, const std::string &account_name, output_logger_manager *output);

private:
    multimap<std::string, void(account_transactions_management::*)() const> available_actions;
    void print_details() const;

    void update_monthly_income() const;
    void set_single_time_income() const;
    void pause_monthly_income() const;
    void restart_monthly_income() const;
    void update_monthly_outcome() const;
    void set_single_time_outcome() const;
    void pause_monthly_outcome() const;
    void restart_monthly_outcome() const;
    void get_outcome_details() const;
    void get_income_details() const;
};

#endif //BANKMANAGEMENT_ACCOUNT_TRANSACTIONS_MANAGEMENT_H
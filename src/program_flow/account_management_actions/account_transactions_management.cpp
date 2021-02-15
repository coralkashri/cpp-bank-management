#include "account_transactions_management.h"
#include "../../extensions/my_shell.h"
#include "../../extensions/std_extensions.h"
#include "../../extensions/custom_exceptions.h"

account_transactions_management::account_transactions_management(db_management *db_ptr, const std::string &account_name,
                                                                 output_logger_manager *output)
                                                                 : account_management_actions_interface(db_ptr, account_name, output) {
    available_actions = {
            {{{"update_monthly_income",   "umi",   ""}, ""}, &account_transactions_management::update_monthly_income},
            {{{"set_single_time_income",  "sin",   ""}, ""}, &account_transactions_management::set_single_time_income},
            {{{"pause_monthly_income",    "pmi",   ""}, ""}, &account_transactions_management::pause_monthly_income},
            {{{"restart_monthly_income",  "rmi",   ""}, ""}, &account_transactions_management::restart_monthly_income},
            {{{"update_monthly_outcome",  "umo",   ""}, ""}, &account_transactions_management::update_monthly_outcome},
            {{{"set_single_time_outcome", "sout",  ""}, ""}, &account_transactions_management::set_single_time_outcome},
            {{{"pause_monthly_outcome",   "pmo",   ""}, ""}, &account_transactions_management::pause_monthly_outcome},
            {{{"restart_monthly_outcome", "rmo",   ""}, ""}, &account_transactions_management::restart_monthly_outcome},
            {{{"get_income_details",      "gind",  ""}, ""}, &account_transactions_management::get_income_details},
            {{{"get_outcome_details",     "goutd", ""}, ""}, &account_transactions_management::get_outcome_details},
    };
}

void account_transactions_management::update_monthly_income() const {
    std::string source_name;
    double cash;
    my_shell::input("source name", source_name);
    my_shell::input("cash", cash);
    db_ptr->update_account_monthly_income(account_name, source_name, cash);
}

void account_transactions_management::set_single_time_income() const {
    std::string source_name;
    double cash;
    my_shell::input("source name", source_name);
    my_shell::input("cash", cash);
    db_ptr->set_account_single_time_income(account_name, source_name, cash);
}

void account_transactions_management::pause_monthly_income() const {
    std::string source_name;
    my_shell::input("source name", source_name);
    db_ptr->pause_account_monthly_income(account_name, source_name);
}

void account_transactions_management::restart_monthly_income() const {
    std::string source_name;
    my_shell::input("source name", source_name);
    db_ptr->restart_account_monthly_income(account_name, source_name);
}

void account_transactions_management::update_monthly_outcome() const {
    std::string target_name;
    double cash;
    my_shell::input("target name", target_name);
    my_shell::input("cash", cash);
    db_ptr->update_account_monthly_outcome(account_name, target_name, cash);
}

void account_transactions_management::set_single_time_outcome() const {
    std::string target_name;
    double cash;
    my_shell::input("target name", target_name);
    my_shell::input("cash", cash);
    db_ptr->set_account_single_time_outcome(account_name, target_name, cash);
}

void account_transactions_management::pause_monthly_outcome() const {
    std::string target_name;
    my_shell::input("target name", target_name);
    db_ptr->pause_account_monthly_outcome(account_name, target_name);
}

void account_transactions_management::restart_monthly_outcome() const {
    std::string target_name;
    my_shell::input("target name", target_name);
    db_ptr->restart_account_monthly_outcome(account_name, target_name);
}

void account_transactions_management::get_outcome_details() const {
    /*int year, month;
    my_shell::input("target year", year);
    my_shell::input("target month", month);*/
    std::vector<transaction> desired_transactions = db_ptr->get_account_outcome_details(account_name, boost::gregorian::date(boost::gregorian::greg_year_month_day{2021, 2, 1}));
    for (auto &t : desired_transactions) {
        std::cout << t << std::endl;
    }
}

void account_transactions_management::get_income_details() const {
    /*int year, month;
    my_shell::input("target year", year);
    my_shell::input("target month", month);*/
    std::vector<transaction> desired_transactions = db_ptr->get_account_income_details(account_name, boost::gregorian::date(boost::gregorian::greg_year_month_day{2021, 2, 1}));
    for (auto &t : desired_transactions) {
        std::cout << t << std::endl;
    }
}

void account_transactions_management::print_details() const {
    output->printer() << "====================================\n";
    output->printer() << "Total Income: " << 0.0 << "\n";
    output->printer() << "Total Outcome: " << 0.0 << "\n";
    output->printer() << "====================================\n";
}
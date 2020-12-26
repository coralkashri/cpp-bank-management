#include "account_transactions_management.h"
#include "../../extensions/my_shell.h"
#include "../../extensions/std_extensions.h"

account_transactions_management::account_transactions_management(db_management *db_ptr, const std::string &account_name,
                                                                 output_logger_manager *output)
                                                                 : db_ptr(db_ptr), account_name(account_name), output(output) {
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
    exit_keyword = "exit";
}

void account_transactions_management::actions() {
    std::string desired_action;
    while (desired_action != exit_keyword) {
        print_details();
        output->printer() << "Available plan actions:\n" << get_available_options() << "\n";
        my_shell::method_input(desired_action);
        if (desired_action != exit_keyword) try {
                apply_action(desired_action);
        } catch (std::exception &e) {
            std::cout << e.what() << "\n";
        }
    }
}

void account_transactions_management::update_monthly_income() const {

}

void account_transactions_management::set_single_time_income() const {

}

void account_transactions_management::pause_monthly_income() const {

}

void account_transactions_management::restart_monthly_income() const {

}

void account_transactions_management::update_monthly_outcome() const {

}

void account_transactions_management::set_single_time_outcome() const {

}

void account_transactions_management::pause_monthly_outcome() const {

}

void account_transactions_management::restart_monthly_outcome() const {

}

void account_transactions_management::get_outcome_details() const {

}

void account_transactions_management::get_income_details() const {

}

void account_transactions_management::apply_action(const std::string &action) const {

}

std::vector<keys_management<std::string>> account_transactions_management::get_available_options() const {
    return std::vector<keys_management<std::string>>();
}

void account_transactions_management::print_details() {
    output->printer() << "====================================\n";
    output->printer() << "Total Income: " << 0.0 << "\n";
    output->printer() << "Total Outcome: " << 0.0 << "\n";
    output->printer() << "====================================\n";
}

#include "plan_management.h"
#include "../extensions/std_extensions.h"
#include "../extensions/custom_exceptions.h"

plan_management::plan_management(db_management *db_ptr, const std::string &account_name, const std::string &plan_name,
                                 output_logger_manager *output)
                                 : db_ptr(db_ptr), account_name(account_name), plan_name(plan_name), output(output) {
    available_actions = {
            {"add_cash", &plan_management::increase_plan_cash},
            {"decrease_cash", &plan_management::decrease_plan_cash},
            {"print_details", &plan_management::print_details}
    };
}

void plan_management::choose_option() {
    auto options = available_actions | std::views::keys | std::views::common;
    std::vector<std::string> options_vec(options.begin(), options.end());
    std::string exit_keyword = "exit";
    options_vec.emplace_back(exit_keyword); // Exit option
    std::string desired_action;

    while (desired_action != exit_keyword) {
        output->printer() << "Available plan actions:\n" << options_vec;
        std::cin >> desired_action;
        try {
            (this->*(available_actions.at(desired_action)))();
        } catch (std::out_of_range &e) {
            throw action_not_found_exception();
        }
    }
}

void plan_management::increase_plan_cash() const {
    double cash;
    std::input("cash to add", cash);
    db_ptr->modify_plan_balance(account_name, plan_name, cash);
}

void plan_management::decrease_plan_cash() const {
    double cash;
    std::input("cash to decrease", cash);
    db_ptr->modify_plan_balance(account_name, plan_name, -cash);
}

void plan_management::print_details() const {
    double cash = db_ptr->get_plan_balance(account_name, plan_name);
    output->printer() << "====================================\n";
    output->printer() << "Account: " << account_name << "\n";
    output->printer() << "Plan: " << plan_name << "\n";
    output->printer() << "Cash: " << cash << "\n";
    output->printer() << "====================================\n";
}

#ifndef BANKMANAGEMENT_ACCOUNT_MANAGEMENT_ACTIONS_INTERFACE_H
#define BANKMANAGEMENT_ACCOUNT_MANAGEMENT_ACTIONS_INTERFACE_H

#include <vector>
#include <string>
#include "../../db/db_management.h"
#include "../../structures/output_logger_manager.h"
#include "../../external_tools/multimap/multimap.h"
#include "../../extensions/my_shell.h"
#include "../../extensions/std_extensions.h"
#include "../../extensions/custom_exceptions.h"

template <typename InheritedClass>
class account_management_actions_interface {
public:

    account_management_actions_interface(db_management *db_ptr, const std::string &account_name,
                                         output_logger_manager *output)
            : db_ptr(db_ptr), account_name(account_name), output(output) {
        exit_keyword = "exit";
    }

    virtual void actions() {
        InheritedClass& derived = static_cast<InheritedClass&>(*this);
        std::string desired_action;
        while (desired_action != exit_keyword) {
            derived.print_details();
            output->printer() << "Available plan actions:\n" << get_available_options() << "\n";
            my_shell::method_input(desired_action);
            if (desired_action != exit_keyword)
                try {
                    apply_action(desired_action);
                } catch (std::exception &e) {
                    std::cout << e.what() << "\n";
                }
        }
    }

protected:
    std::string account_name;
    db_management *db_ptr;
    output_logger_manager *output;
    std::string exit_keyword;

    void apply_action(const std::string &action) const {
        try {
            const InheritedClass& derived = static_cast<const InheritedClass&>(*this);
            (derived.*(derived.available_actions.at(action)))();
        } catch (std::out_of_range &e) {
            throw action_not_found_exception();
        }
    }

    [[nodiscard]] std::vector<keys_management<std::string>> get_available_options() const {
        const InheritedClass& derived = static_cast<const InheritedClass&>(*this);
        auto options = derived.available_actions.get_key_relations();
        std::vector<keys_management<std::string>> options_vec(options.begin(), options.end());
        options_vec.emplace_back(keys_management<std::string>{exit_keyword}); // Exit option
        return options_vec;
    }

private:
    virtual void print_details() const = 0;
};

#endif //BANKMANAGEMENT_ACCOUNT_MANAGEMENT_ACTIONS_INTERFACE_H
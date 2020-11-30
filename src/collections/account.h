#ifndef BANKMANAGEMENT_ACCOUNT_H
#define BANKMANAGEMENT_ACCOUNT_H

#include <string>
#include <vector>
#include <filesystem>
#include "plan.h"

class account {
public:
    account(const std::string &db_path, const std::string &account_name);

    [[nodiscard]] std::string get_account_name() const;
    [[nodiscard]] std::vector<std::string> get_plan_names() const;
    void print_plan_details(const std::string &plan_name);
    void create_plan(const std::string &plan_name);
    void plan_management(const std::string &plan_name);
    void remove_plan(std::string plan_name);

    bool operator==(const std::string &name);
    bool operator!=(const std::string &name);

private:
    template<typename T>
    using plans_container = std::vector<T>;
    plans_container<plan> plans;
    std::string account_name;
    std::string db_path;

    plans_container<plan>::iterator find_plan(const std::string &name);
    std::filesystem::path dir_path();
};

#endif //BANKMANAGEMENT_ACCOUNT_H
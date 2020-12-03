#ifndef BANKMANAGEMENT_PLAN_H
#define BANKMANAGEMENT_PLAN_H

#include <string>

class plan {
public:
    plan(const std::string &db_path, std::string plan_name);
    ~plan();

    bool delete_plan();

    std::string get_plan_name() const;
    void print_details() const;
    void plan_management();

    bool operator==(const std::string &name);
    bool operator!=(const std::string &name);

private:
    std::string plan_name;
    std::string db_path;
    std::filesystem::path dir_path();
};

#endif //BANKMANAGEMENT_PLAN_H
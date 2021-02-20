#ifndef BANKMANAGEMENT_FILTER_FIELD_H
#define BANKMANAGEMENT_FILTER_FIELD_H

#include <string>
#include <map>
#include <variant>

class filter_field {
public:
    using optional_variable_types = std::variant<std::string, int, bool, float, double>;

    explicit filter_field(std::string &&field_hierarchy);

    filter_field(std::string &&field_hierarchy, std::map<std::string, optional_variable_types> &&sub_fields);

    filter_field& add_sub_field(std::string &&field_name, optional_variable_types &&value);

    [[nodiscard]] std::string get_field_hierarchy() const;

    [[nodiscard]] std::map<std::string, optional_variable_types> get_sub_fields() const;

private:
    std::string field_hierarchy;
    std::map<std::string, optional_variable_types> sub_fields;
};

#endif //BANKMANAGEMENT_FILTER_FIELD_H

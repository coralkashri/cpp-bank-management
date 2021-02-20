#include "filter_field.h"

filter_field::filter_field(std::string &&field_hierarchy) : field_hierarchy(field_hierarchy) {}

filter_field::filter_field(std::string &&field_hierarchy, std::map<std::string, optional_variable_types> &&sub_fields)
        : field_hierarchy(field_hierarchy), sub_fields(sub_fields) {}

filter_field &filter_field::add_sub_field(std::string &&field_name, filter_field::optional_variable_types &&value) {
    sub_fields.insert({field_name, value});
    return *this;
}

std::string filter_field::get_field_hierarchy() const {
    return field_hierarchy;
}

std::map<std::string, filter_field::optional_variable_types> filter_field::get_sub_fields() const {
    return sub_fields;
}

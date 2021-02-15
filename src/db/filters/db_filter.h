#ifndef BANKMANAGEMENT_DB_FILTER_H
#define BANKMANAGEMENT_DB_FILTER_H

#include <bsoncxx/builder/basic/document.hpp>

class db_filter {
public:
    template <size_t FilterType, typename ConcreteFilter, typename ...Args>
    const db_filter& apply(const ConcreteFilter &concrete_filter, bsoncxx::builder::basic::document &document_filter,
                           const std::string &field_hierarchy, Args&& ...args) const {
        concrete_filter.template apply<FilterType>(document_filter, field_hierarchy, std::forward<Args>(args)...);
        return *this;
    }

    template <size_t FilterType, typename ConcreteFilter, typename ...Args>
    const db_filter& cond_apply(bool is_apply, const ConcreteFilter &concrete_filter, bsoncxx::builder::basic::document &document_filter,
                           const std::string &field_hierarchy, Args&& ...args) const {
        if (is_apply)
            concrete_filter.template apply<FilterType>(document_filter, field_hierarchy, std::forward<Args>(args)...);
        return *this;
    }
};

#endif //BANKMANAGEMENT_DB_FILTER_H
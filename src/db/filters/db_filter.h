#ifndef BANKMANAGEMENT_DB_FILTER_H
#define BANKMANAGEMENT_DB_FILTER_H

#include <bsoncxx/builder/basic/document.hpp>
#include <variant>

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

    /**
     *
     * @tparam Args
     * @param filter
     * @param array_hierarchy
     * @param fields .first  -> Field name
     *               .second -> Field param
     */
    template <typename ...Args>
    static void make_regular_filter(bsoncxx::builder::basic::document &filter, const std::string &array_hierarchy,
                                    std::pair<std::string, Args>&& ...fields) {
        using bsoncxx::builder::basic::kvp;
        using bsoncxx::builder::basic::sub_document;
        using optional_variable_types = std::variant<std::string, int, bool, float, double>;

        std::vector<std::pair<std::string, optional_variable_types>> fields_arr = {fields...};

        filter.append(kvp(array_hierarchy, [&fields_arr](sub_document transactions) {
            transactions.append(kvp("$elemMatch", [&](sub_document $elem_match) {
                for (auto &field : fields_arr) {
                    $elem_match.append(kvp(field.first, [&](sub_document transaction_name_id) {
                        std::visit([&](auto &&arg) {
                            transaction_name_id.append(kvp("$eq", arg));
                        }, field.second);
                    }));
                }
            }));
        }));
    }

    template <typename ValueT>
    static void make_aggregate_filter(bsoncxx::builder::basic::document &filter, const std::string &array_hierarchy,
                               const std::string &field_name, ValueT &&val) {
        using bsoncxx::builder::basic::kvp;
        using bsoncxx::builder::basic::sub_document;
        using bsoncxx::builder::basic::sub_array;

        filter.append(kvp(array_hierarchy, [&](sub_document transactions) {
            transactions.append(kvp("$filter", [&](sub_document $filter) {
                $filter.append(kvp("input", "$" + array_hierarchy));
                std::string it_name = "it";
                $filter.append(kvp("as", it_name));
                $filter.append(kvp("cond", [&](sub_document cond) {
                    cond.append(kvp("$eq", [&](sub_array $eq) {
                        $eq.append("$$" + it_name + "." + field_name, val);
                    }));
                }));
            }));
        }));
    }
};

#endif //BANKMANAGEMENT_DB_FILTER_H
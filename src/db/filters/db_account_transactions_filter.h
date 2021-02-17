#ifndef BANKMANAGEMENT_DB_ACCOUNT_TRANSACTIONS_FILTER_H
#define BANKMANAGEMENT_DB_ACCOUNT_TRANSACTIONS_FILTER_H

#include <iostream>
#include <bsoncxx/builder/basic/document.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include "../../program_flow/structures/transaction.h"

class db_account_transactions_filter {
public:
    enum filter_types {
        ID,
        IS_INCOME,
        ARCHIVE_BY_DATE,
        AGR_IS_INCOME,
        AGR_ARCHIVE_BY_DATE,
    };

public:
    explicit db_account_transactions_filter(std::string transactions_db_field_name);

    template <size_t Type, typename ...Args>
    void apply(bsoncxx::builder::basic::document &filter, const std::string &field_hierarchy, Args&& ...args) const {
        if constexpr (Type == ID)
            filter_by_id(filter, field_hierarchy, std::forward<Args>(args)...);

        else if constexpr (Type == IS_INCOME)
            filter_by_is_income(filter, field_hierarchy, std::forward<Args>(args)...);

        else if constexpr (Type == ARCHIVE_BY_DATE)
            filter_archive_by_date(filter, field_hierarchy, std::forward<Args>(args)...);

        else if constexpr (Type == AGR_IS_INCOME)
            aggregate_filter_by_is_income(filter, field_hierarchy, std::forward<Args>(args)...);

        else if constexpr (Type == AGR_ARCHIVE_BY_DATE)
            aggregate_filter_archive_by_date(filter, field_hierarchy, std::forward<Args>(args)...);
    }

private:
    std::string transactions_db_field_name;

    void filter_by_id(bsoncxx::builder::basic::document &filter, const std::string &transactions_hierarchy,
                      const transaction_id &t_id) const;

    void filter_by_is_income(bsoncxx::builder::basic::document &filter, const std::string &transactions_hierarchy,
                             bool is_income) const;

    void filter_archive_by_date(bsoncxx::builder::basic::document &filter, const std::string &archive_hierarchy,
                                const boost::gregorian::date &date) const;

    void aggregate_filter_by_is_income(bsoncxx::builder::basic::document &filter, const std::string &transactions_hierarchy,
                             bool is_income) const;

    void aggregate_filter_archive_by_date(bsoncxx::builder::basic::document &filter, const std::string &archive_hierarchy,
                                const boost::gregorian::date &date) const;

    template <typename ValueT>
    void make_regular_filter(bsoncxx::builder::basic::document &filter, const std::string &array_hierarchy,
                             const std::string &field_name, const ValueT &val) const {
        using bsoncxx::builder::basic::kvp;
        using bsoncxx::builder::basic::sub_document;

        filter.append(kvp(array_hierarchy, [&field_name, &val, this](sub_document transactions) {
            transactions.append(kvp("$elemMatch", [&](sub_document $elem_match) {
                $elem_match.append(kvp(field_name, [&](sub_document transaction_name_id) {
                    transaction_name_id.append(kvp("$eq", val));
                }));
            }));
        }));
    }

    template <typename ValueT>
    void make_aggregate_filter(bsoncxx::builder::basic::document &filter, const std::string &array_hierarchy,
                               const std::string &field_name, const ValueT &val) const {
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

#endif //BANKMANAGEMENT_DB_ACCOUNT_TRANSACTIONS_FILTER_H
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
};

#endif //BANKMANAGEMENT_DB_ACCOUNT_TRANSACTIONS_FILTER_H
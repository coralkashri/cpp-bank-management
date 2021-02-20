#include "db_filter.h"

void db_filter::make_regular_filter_on_arrays(bsoncxx::builder::basic::document &filter, const std::vector<filter_field> &fields) {
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::sub_document;

    for (auto const& field : fields) {
        filter.append(kvp(field.get_field_hierarchy(), [fields = field.get_sub_fields()](sub_document array_document) {
            array_document.append(kvp("$elemMatch", [&](sub_document $elem_match) {
                for (auto const&[field_name, field_data] : fields) {
                    $elem_match.append(kvp(field_name, [&](sub_document transaction_name_id) {
                        std::visit([&](auto &&arg) {
                            transaction_name_id.append(kvp("$eq", arg));
                        }, field_data);
                    }));
                }
            }));
        }));
    }
}
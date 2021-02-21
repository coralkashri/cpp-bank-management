#include "boost_gregorian_extensions.h"

namespace boost::gregorian {
    date current_local_date() {
        return boost::gregorian::date(boost::gregorian::day_clock::local_day_ymd());
    }
}
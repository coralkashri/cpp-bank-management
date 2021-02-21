#ifndef BANKMANAGEMENT_RANGES_EXTENSIONS_H
#define BANKMANAGEMENT_RANGES_EXTENSIONS_H

#include <ranges>

namespace std::ranges::views {

    template <template <typename...> typename DesiredCont, typename T, typename Range>
    DesiredCont<T> convert_common_to_container(Range r) {
        return DesiredCont<T>(r.begin(), r.end());
    }

}

#endif //BANKMANAGEMENT_RANGES_EXTENSIONS_H
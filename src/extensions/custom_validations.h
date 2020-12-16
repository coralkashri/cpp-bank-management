#ifndef BANKMANAGEMENT_CUSTOM_VALIDATIONS_H
#define BANKMANAGEMENT_CUSTOM_VALIDATIONS_H

#include "custom_concepts.h"

namespace custom_validations {

    template<Numerical T>
    bool is_positive(T num) {
        return num >= 0;
    }
}

#endif //BANKMANAGEMENT_CUSTOM_VALIDATIONS_H
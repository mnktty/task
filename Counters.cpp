#include "Counters.h"

namespace Counters {
    uint32_t counters_[MSG_MAX_TYPE];

    void print(std::ostream& os) {
        for (int i = 0; i < MSG_MAX_TYPE; ++i) {
            os << "Counter[" << eMessageTypeNames[i] << "] = " << counters_[i] << '\n';
        }
    }
}

// eof

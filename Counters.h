#ifndef _COUNTERS_H_
#define _COUNTERS_H_

#include "Common.h"
#include <ostream>
#include <stdint.h>

namespace Counters {
    extern uint32_t counters_[MSG_MAX_TYPE];
    
    inline uint32_t* at(const uint32_t index) { 
        return &counters_[index];
    }

    void print(std::ostream& os);
    
}


#endif /* _COUNTERS_H_ */

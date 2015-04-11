#include "Common.h"

const char* eMessageTypeNames[] = {
    "MSG_SHUTDOWN",               /* used to shutdown any task */
    "MSG_CONFIG",
    "MSG_REQUEST",                /* other kinds of messages... */
    "MSG_RESPONSE",
    "MSG_TIMER",
    "MSG_MAX_TYPE"
};

// eof

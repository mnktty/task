#ifndef _DEFTYPES_H_
#define _DEFTYPES_H_

/**
 * @file Common.h
 *
 * */

/**
 * Types of messages */
enum eMessageType {
    MSG_SHUTDOWN,               /* used to shutdown any task */
    MSG_CONFIG,
    MSG_REQUEST,                /* other kinds of messages... */
    MSG_RESPONSE,
    MSG_TIMER,
    MSG_MAX_TYPE
};

extern const char* eMessageTypeNames[];

/* stringify */
#define STR(x) (#x)

#endif /* _DEFTYPES_H_ */

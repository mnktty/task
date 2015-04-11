#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "Common.h"
#include "Counters.h"
#include <stdint.h>

class Message {
    uint32_t id_;
    eMessageType type_;
    
    /* Add any other stuff here - e.g. a payload */

public:

    Message(const uint32_t id, eMessageType type): id_(id), type_(type) {}

    // default ctor creates the sentinel object
    Message(): id_(0), type_(MSG_SHUTDOWN) {}

    eMessageType type() const {
        return type_;
    }

    void process();
};

void Message::process() {
    switch (type_) {
    case MSG_CONFIG:
        __sync_fetch_and_add(Counters::at(MSG_CONFIG), 1);
        LOG_INFO("id: " << id_ << "," << STR(MSG_CONFIG) << "\n");
        break;

    case MSG_REQUEST:
        __sync_fetch_and_add(Counters::at(MSG_REQUEST), 1);
        LOG_INFO("id: " << id_ << "," << STR(MSG_REQUEST) << "\n");
        break;

    case MSG_RESPONSE:
        __sync_fetch_and_add(Counters::at(MSG_RESPONSE), 1);
        LOG_INFO("id: " << id_ << "," << STR(MSG_RESPONSE) << "\n");
        break;
 
    case MSG_TIMER:
        __sync_fetch_and_add(Counters::at(MSG_TIMER), 1);
        LOG_INFO("id: " << id_ << "," << STR(MSG_TIMER) << "\n");
        break;

    default:
        LOG_INFO("Ignoring unknown message\n");
    }
}


#endif /* _MESSAGE_H_ */

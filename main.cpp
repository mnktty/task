#include "Task.h"
#include "Message.h"

#include <signal.h>
#include <cstdlib>
#include <cassert>
#include <iostream>

enum eThreadTypes { 
    MAIN = 0,
    CONFIG,
    MAX_THREADS
};

void runDispatcher() {

    // this could be a task group
    Task<Message> tasks[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; ++i)
    {
        tasks[i].start();        
    } 

    Message* m;

    m = new Message(1, MSG_REQUEST);
    tasks[MAIN].enqueue(m);
    m = new Message(1, MSG_RESPONSE);
    tasks[MAIN].enqueue(m);
 
    m = new Message(20, MSG_CONFIG);
    tasks[CONFIG].enqueue(m);
 
    m = new Message(2, MSG_REQUEST);
    tasks[MAIN].enqueue(m);
    m = new Message(2, MSG_RESPONSE);
    tasks[MAIN].enqueue(m);
 
    m = new Message(10, MSG_TIMER);
    tasks[CONFIG].enqueue(m);
    m = new Message(11, MSG_TIMER);
    tasks[CONFIG].enqueue(m);
    m = new Message(12, MSG_TIMER);
    tasks[CONFIG].enqueue(m);
 
    // so that tasks gets a breather to process everything before you call stop
    sleep(1);

    for (int i = 0; i < MAX_THREADS; ++i) {
        tasks[i].stop();
        tasks[i].join();
    }
    
    Counters::print(std::cout);
}


int main(int argc, char *argv[])
{
    runDispatcher();
    return 0;
}

// eof

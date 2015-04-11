#ifndef _TASK_H_
#define _TASK_H_

#include "Common.h"
#include "Log.h"
#include <queue>
#include <cassert>
#include <pthread.h>

static int UPPER_WATERMARK = 100;

/**
 * An active object that blocks for tasks and runs to completion (one queue, one
 * thread of execution). Any type of MSG which conforms to @see Message may be
 * used to enqueue to this task type. */
template <typename MSG>
class Task {
    volatile bool active_;

    /* atomic queue of messages */
    std::queue<MSG*> queue_;
    pthread_mutex_t mutex_;
    pthread_cond_t  condition_;

    /* thread handle */
    pthread_t thread_;

    /* thread function and its delegates */
    static void* thread_func(void* arg) {
        static_cast<Task<MSG>*>(arg)->run(); 
    }

    void run();
    void consume(MSG* msg);
  

public:

    Task();
    ~Task();

    void start();               /* start this task */
    void stop();                /* stop this task */
    void join();                /* wait for this task to cleanup after stop */

    void enqueue(MSG* const msg); /* give a message to process by this task */

};

template <typename MSG>
Task<MSG>::Task() : active_(false) {
    int status;

    status = ::pthread_mutex_init(&mutex_, NULL);
    assert(0 == status);

    status = ::pthread_cond_init(&condition_, NULL);
    assert(0 == status);
}

template <typename MSG>
Task<MSG>::~Task() {

    // clear any pending messages (to be processed), we don't execute them
    while(!queue_.empty()) {
        MSG* msg = queue_.front();
        delete msg;
        msg = 0;
        queue_.pop();
    }

    int status;
    status = ::pthread_cond_destroy(&condition_);
    assert(0 == status);

    status = ::pthread_mutex_destroy(&mutex_);
    assert(0 == status);
}

template <typename MSG>
void Task<MSG>::start() {
    int status = ::pthread_create(&thread_, NULL, &Task<MSG>::thread_func, 
                                  static_cast<void*>(this));
    assert(0 == status);
}

template <typename MSG>
void Task<MSG>::join() {
    int status = ::pthread_join(thread_, NULL);
    assert(0 == status);
}

template <typename MSG>
void Task<MSG>::enqueue(MSG* const msg) {
    ::pthread_mutex_lock(&mutex_);
    while (queue_.size() > UPPER_WATERMARK) {
        LOG_TRACE("ps");
        ::pthread_cond_wait(&condition_, &mutex_);
        LOG_TRACE("pw");
    }

    queue_.push(msg);
    LOG_TRACE("+");

    ::pthread_cond_signal(&condition_);
    ::pthread_mutex_unlock(&mutex_);
}

template <typename MSG>
void Task<MSG>::run() {

    active_ = true;
    while(active_) {
        ::pthread_mutex_lock(&mutex_);
        while (queue_.empty()) {
            LOG_TRACE("cs");
            ::pthread_cond_wait(&condition_, &mutex_);
            LOG_TRACE("cw");
        }
        
        MSG* msg = queue_.front();
        queue_.pop();
        LOG_TRACE("-");
        
        ::pthread_cond_signal(&condition_);
        ::pthread_mutex_unlock(&mutex_);
        
        consume(msg);
    }
}


template <typename MSG>
void Task<MSG>::consume(MSG* msg) {
    if (msg) {
        switch(msg->type()) {
        case MSG_SHUTDOWN:
            active_ = false;
            break;
        
        default:
            msg->process();
            break;

        }
    }
    
    // finally consume it
    delete msg;
    msg = 0;
}


template <typename MSG>
void Task<MSG>::stop() {
    active_ = false;
    /* enqueue a sentinel message */
    enqueue(new MSG());
}

#endif /* _TASK_H_ */

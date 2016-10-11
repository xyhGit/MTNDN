//
//  ndnrtc-object.h
//  ndnrtc
//
//  Copyright 2013 Regents of the University of California
//  For licensing details see the LICENSE file.
//
//  Author:  Peter Gusev 
//  Created: 8/21/13
//

#define BOOST_THREAD_USE_LIB

#ifndef _object__
#define _object__

#include <boost/thread.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/atomic.hpp>

#include <mutex>
#include <thread>
#include <ndn-cpp/common.hpp>

using namespace ndn;

//#include "simple-log.h"
//#include "params.h"
//#include "interfaces.h"

class INdnRtcComponentCallback {
public:
    virtual ~INdnRtcComponentCallback(){}
    virtual void onError(const char *errorMessage,
                            const int errorCode = -1) = 0;
};
        
class NdnRtcComponent :
		public INdnRtcComponentCallback,
		public boost::enable_shared_from_this<NdnRtcComponent>
{
public:
    // construction/desctruction
    NdnRtcComponent();
    NdnRtcComponent(INdnRtcComponentCallback *callback);
    virtual ~NdnRtcComponent();

    virtual void registerCallback(INdnRtcComponentCallback *callback)
    { callback_ = callback; }
    virtual void deregisterCallback()
    { callback_ = NULL; }
            
    virtual void onError(const char *errorMessage, const int errorCode);
/*
    // ILoggingObject interface conformance
    virtual std::string
    getDescription() const;
            
    virtual bool
    isLoggingEnabled() const
    { return true; }
*/

protected:
    ptr_lib::atomic<bool> isJobScheduled_;
    ptr_lib::mutex callbackMutex_;
    ptr_lib::recursive_mutex jobMutex_;
    INdnRtcComponentCallback *callback_ = nullptr;
            

    // protected methods go here
    int notifyError(const int ecode, const char *format, ...);
    bool hasCallback() { return callback_ != NULL; }
            
    ptr_lib::thread startThread(ptr_lib::function<bool()> threadFunc);
    void stopThread(ptr_lib::thread &thread);
    void scheduleJob(const unsigned int usecInterval,
                        ptr_lib::function<bool()> jobCallback);
    void stopJob();

private:
    bool isTimerCancelled_;
    boost::asio::steady_timer watchdogTimer_;
};

#endif /* defined(_object__) */

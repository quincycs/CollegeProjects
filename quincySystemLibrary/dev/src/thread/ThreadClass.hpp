/*
 * ThreadClass.hpp
 *
 *  Created on: Feb 3, 2010
 *      Author: quincy
 */

#ifndef THREADCLASS_HPP_
#define THREADCLASS_HPP_



#include "Thread.hpp"
#include "Mutex.hpp"
#include <iostream>
using std::cerr;
using std::endl;

namespace quincySystemLibrary {

/**
 * This class's purpose is to provide a subclass all the tools to spawn, cancel, set cancellation points
 * join without needing to use the Thread class directly.
 *
 */
class ThreadClass {

private:
	/// underlying thread entry point calls live(), scheduleJoin() then thread waits to be joined.
	static THREADRETURN threadStarter(void *threadParams);
	/// underlying mutex that gets locked for ScopeNoCancelSection() and Cancel()
	Mutex criticalSection;
	/// underlying flag used to implement isRunning().
	bool running;
protected:
	/// underlying thread structure
	Thread *thread;
	/// creates underlying thread structure, but does not start thread.
	ThreadClass();

	/**
	 * this method is the last words of the thread before it waits to be joined.
	 * It is called during cancel() and after the thread returns.
	 * It is recommended that only operations of notifying another thread to call join() should be done.
	 */
	virtual void scheduleJoin()=0;
	/// thread entry point method which is called by threadStarter()
	virtual void live() = 0;
public:
	///The thread is started, and it's entry point is the live() method in the inherited class.
	void start(){running=true;thread->start();}
	/**
	 * flag turned to let user know whether to cancel and join this object for clean up.
	 * flag is turned false just before scheduleJoin() is called. if the thread does not hit
	 *  a system defined cancelation point frequently, then this method call can be used by
	 *  the thread to check if it was canceled and act accordingly.
	 */
	bool isRunning(){return running;}

	/**
	 * blocks until thread is in an able to cancel scope, then a cancel is performed on the thread and
	 * scheduleJoin() is called.  WARNING: It is potentially inefficient to have an empty scheduleJoin()
	 * implementation and call Join() right after a call to cancel.  The cancel on the underlying thread
	 * could take a significant amount of cpu cycles.
	 */
	void cancel();
	/**
	 * join the underlying thread. (blocking).  Ideally this call should have been scheduled by
	 * scheduleJoin() so that this method would return immediately and not have to block.
	 */
	void join();
	virtual ~ThreadClass();

	/**
	 * To set a critical section of code which can not be canceled then call this method
	 * to set a the caller's scope of code not able to be canceled.
	 *
	 * example application:
	 *		//while(1){
	 * 		//	some sleeping function or blocking read
	 * 		//	 { //create new scope of code not able to cancel.
	 * 		//		ScopedLock lock = ScopeNoCancelSection();
	 * 		//		///code that can't be canceled
	 * 		//	 } //lock out of scope so it unlocks mutex
	 * 		//}
	 * Note: if return value is not set then mutex is unlocked after this call.
	 * see rational of ScopedLock in Thread.hpp
	 */
	ScopedLock scopeNoCancelSection();
};

}

#endif /* THREADCLASS_HPP_ */

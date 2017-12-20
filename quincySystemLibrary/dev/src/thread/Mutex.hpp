/*
 * Mutex.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#ifndef MUTEX_HPP_
#define MUTEX_HPP_

#include "posix/MutexImp.hpp" // USE_POSIX_THREADS

namespace quincySystemLibrary {

/// classic mutex which only locks/unlocks a thread from context switching with other threads executing the same execution code.
class Mutex : private MutexImp{
public:
	///creates mutex into unlocked state
	Mutex(){}
	virtual ~Mutex(){}

	///set the start of a critical section for this thread.
	void lock(){MutexImp::lock();}
	///set end of critical section for this thread. (Careful: thread B can not unlock thread A's critical section.  You should only call unlock on the same thread and mutex as you did for lock).
	void unlock(){MutexImp::unlock();}
};

/**
 * When a ScopeLock is out of scope then it will be unlocked and not destroyed.
 *
 * Here is a common situation: Using a mutex and there are many points of failure/points of function returning in method and the mutex needs to be unlocked after those points.
 *  Instead of the solution of the following...
	    Mutex mtx; is a private class variable:

		mtx.lock();
		try{
			// some code that needs to be in a critical section
			// many points of failure/points of function returning/exceptions thrown.
		}
		catch (...){
			mtx.unlock();
			throw;
		}
		mtx.unlock();
 *
 * Do this instead:
 *	{
 *  ScopeLock lock(mtx);
 * 	// some code that needs to be in a critical section
 *  // many points of failure/points of function returning/exceptions thrown.
 *  // if any of this critical section returns or fails, the ScopeLock will go out of scope and unlock.
 *  }
 *  //lock is out of scope now, so that means the mutex is unlocked.
 */
/// lock a mutex only when ScopedLock stays in scope.
class ScopedLock {
private:
	Mutex &_mutex;
public:
	///locks mutex
	ScopedLock(Mutex &mutex):_mutex(mutex){mutex.lock();}
	///unlocks mutex
	~ScopedLock(){_mutex.unlock();}
};

}

#endif /* MUTEX_HPP_ */

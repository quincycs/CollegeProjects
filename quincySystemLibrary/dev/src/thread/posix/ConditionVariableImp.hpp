/*
 * ConditionVariableImp.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#ifdef USE_POSIX_THREADS
#ifndef CONDITIONVARIABLEIMP_HPP_
#define CONDITIONVARIABLEIMP_HPP_

#include <pthread.h>
#include "thread/posix/MutexImp.hpp"

namespace quincySystemLibrary {

class Mutex;

class ConditionVariableImp {
private:
	MutexImp &_mutex;
	pthread_cond_t cond;
public:
	ConditionVariableImp(Mutex &mutex);
	virtual ~ConditionVariableImp();

	///signal that the shared data has reached an interesting state which another thread should wake up and handle.  this interesting shared data state is called the predict condition.
	void signal();
	///mutex should be locked before calling this.  mutex will be atomically unlocked inside this method,and once signal is caught the mutex will be atomically locked again and then this method will return. CAREFUL: make sure you are checking the predict condition after this method.  Catching a signal does not imply the predict condition is true due to the possibility of many wake ups of different threads on one signal.
	void wait();
};

}

#endif /* CONDITIONVARIABLEIMP_HPP_ */
#endif

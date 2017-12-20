/*
 * MutexImp.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#ifdef USE_POSIX_THREADS

#ifndef MUTEXIMP_HPP_
#define MUTEXIMP_HPP_

#include <pthread.h>

namespace quincySystemLibrary {

class ConditionVariableImp;

class MutexImp {
private:
	pthread_mutex_t mutex;
public:
	MutexImp();
	virtual ~MutexImp();

	void lock();
	void unlock();

	friend class ConditionVariableImp;
};

}

#endif /* MUTEXIMP_HPP_ */
#endif

/*
 * ConditionVariableImp.cpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#include "ConditionVariableImp.hpp"

#ifdef USE_POSIX_THREADS


namespace quincySystemLibrary {

ConditionVariableImp::ConditionVariableImp(Mutex &mutex):_mutex((MutexImp&)mutex) {

	pthread_cond_init(&cond,NULL);
}

ConditionVariableImp::~ConditionVariableImp() {
	pthread_cond_destroy(&cond);
}

void ConditionVariableImp::wait(){
	pthread_cond_wait(&cond,&(_mutex.mutex));
}

void ConditionVariableImp::signal(){
	pthread_cond_signal(&cond);
}

}

#endif


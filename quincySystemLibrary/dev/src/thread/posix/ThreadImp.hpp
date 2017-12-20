/*
 * ThreadImp.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */
#ifdef USE_POSIX_THREADS
#ifndef THREADIMP_HPP_
#define THREADIMP_HPP_

#include <pthread.h>

#define THREADRETURN void *
typedef THREADRETURN( *THREADFUNC)(void*);

namespace quincySystemLibrary {

class ThreadImp {
private:
	pthread_t threadHandle;

	THREADFUNC func;
	void *data;
public:
	/// give static function which has THREADRETURN for it's return type and void* for it's only argument. dynamically allocate 'data' field (which you are responsible to delete) or set to NULL.
	ThreadImp(THREADFUNC func, void *data){this->func=func;this->data=data;}
	virtual ~ThreadImp(){}

	void start();
	THREADRETURN join();

	void cancel();
};

}

#endif /* THREADIMP_HPP_ */
#endif

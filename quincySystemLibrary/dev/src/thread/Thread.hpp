/*
 * Thread.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#ifndef THREAD_HPP_
#define THREAD_HPP_

#include "posix/ThreadImp.hpp"//USE_POSIX_THREADS

namespace quincySystemLibrary {

class Thread : private ThreadImp {
public:
	/// give a entry function for thread to start at which has THREADRETURN as the return type and correct arguments specified by the thread implementation.  data field should be null or dynamically allocated.
	Thread(THREADFUNC func, void *data=NULL):ThreadImp(func,data){}
	/// deletes data field
	virtual ~Thread(){}

	void start(){ThreadImp::start();}

	THREADRETURN join(){return ThreadImp::join();}

	void cancel(){ThreadImp::cancel();}
};

}

#endif /* THREAD_HPP_ */

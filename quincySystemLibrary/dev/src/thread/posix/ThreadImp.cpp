/*
 * ThreadImp.cpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#include "ThreadImp.hpp"

namespace quincySystemLibrary {



void ThreadImp::start(){
	pthread_create(&threadHandle, NULL, func, data);
}

THREADRETURN ThreadImp::join(){
	void *value_ptr;
	pthread_join(threadHandle,&value_ptr);
	return value_ptr;
}

void ThreadImp::cancel(){
	pthread_cancel(threadHandle);
}

}

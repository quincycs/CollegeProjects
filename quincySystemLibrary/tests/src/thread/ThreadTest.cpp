/*
 * ThreadTest.cpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#include "ThreadTest.hpp"

namespace quincySystemLibrary_Tests {

THREADRETURN ThreadTest::threadStarter(void *arg){

	return arg;
}

void ThreadTest::RunTests()throw(exception){
	char a='a';
	Thread *thread = new Thread((THREADFUNC)threadStarter,&a);
	thread->start();
	char *a_ptr = (char*)thread->join();
	assert_message("thread return value not correct",a==*a_ptr);
}

}

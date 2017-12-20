/*
 * ThreadClass.cpp
 *
 *  Created on: Feb 3, 2010
 *      Author: quincy
 */

#include "ThreadClass.hpp"

namespace quincySystemLibrary {

ThreadClass::ThreadClass() {

	thread = new Thread((THREADFUNC)ThreadClass::threadStarter, (void*) this);
	running=false;
}

THREADRETURN ThreadClass::threadStarter(void *instanceVar){
	ThreadClass *instance = (ThreadClass*)instanceVar;
	//cerr << "thread started "<<instance << endl;
	instance->live();

	instance->running=false;
	instance->scheduleJoin();

	return NULL;
}

void ThreadClass::cancel(){

	//once mux is gotten the thread has agreed that it is available to cancel.
	criticalSection.lock();

	thread->cancel();
	running=false;
	scheduleJoin();

	criticalSection.unlock();
}

ScopedLock ThreadClass::scopeNoCancelSection(){

	return ScopedLock(criticalSection);
}

void ThreadClass::join(){

	thread->join();
	//cerr << "thread joined " <<this << endl;
}

ThreadClass::~ThreadClass() {

	if(thread!=NULL){
		delete thread;
		thread=NULL;
	}
}

}

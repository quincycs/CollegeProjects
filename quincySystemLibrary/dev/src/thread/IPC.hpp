/*
 * IPC.hpp
 *
 *  Created on: Jul 30, 2009
 *      Author: quincy
 */

#ifndef IPC_HPP_
#define IPC_HPP_


#include "Mutex.hpp"
#include "ConditionVariable.hpp"

#include <queue>
using std::queue;
#include <stdint.h>

namespace quincySystemLibrary {

///Inter-Process Communication of a certain data type T.  Allows many senders and many receivers.
template <class T>
class IPC {

private:
	bool begin_wait;
	Mutex mutexWaiters;
	ConditionVariable *monitorWaitersCond;//use to make sure only one thread is waiting for signal.

	queue<T*> q;//queue of sent data
	ConditionVariable *cond;
	Mutex mutex;

	void allowOnlyOneReceiver();
	void signalWaiters();

public:
	IPC();
	virtual ~IPC();

	///non-blocking. stores into a queue and signals any sleeping threads in recv().
	void send(T* tType);

	/**
	 * blocking. blocks if queue is empty by putting thread to sleep
	 * and waking up when a send() signals this thread.
	 */
	T* recv();

	uint32_t queueLength(){return q.size();}
};



template <class T>
IPC<T>::IPC() {
	begin_wait=false;
	cond = new ConditionVariable(mutex);
	monitorWaitersCond = new ConditionVariable(mutexWaiters);
}

template <class T>
IPC<T>::~IPC(){}

template <class T>
void IPC<T>::send(T* tType_Arg){

	mutex.lock();
	q.push(tType_Arg);
	mutex.unlock();
	cond->signal();
}

template <class T>
T* IPC<T>::recv(){

	allowOnlyOneReceiver();

	T* t;

	mutex.lock();
	while(q.empty()){
		cond->wait();
	}
	t = q.front();
	q.pop();
	mutex.unlock();

	signalWaiters();
	return t;
}


template <class T>
void IPC<T>::allowOnlyOneReceiver(){
	mutexWaiters.lock();

	while(begin_wait){
		monitorWaitersCond->wait();
	}
	begin_wait=true;
	mutexWaiters.unlock();
}

template <class T>
void IPC<T>::signalWaiters(){
	begin_wait=false;
	monitorWaitersCond->signal();
}

}

#endif /* IPC_HPP_ */


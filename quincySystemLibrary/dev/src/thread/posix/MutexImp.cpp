/*
 * MutexImp.cpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#include "MutexImp.hpp"

namespace quincySystemLibrary {

MutexImp::MutexImp() {
	pthread_mutex_init(&mutex,NULL);
}

MutexImp::~MutexImp() {
	pthread_mutex_destroy(&mutex);
}

void MutexImp::lock(){
	pthread_mutex_lock(&mutex);
}

void MutexImp::unlock(){
	pthread_mutex_unlock(&mutex);
}

}

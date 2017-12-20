/*
 * ThreadClassTest.cpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#include "ThreadClassTest.hpp"

namespace quincySystemLibrary_Tests {

void ThreadClassTest::RunTests()throw(exception){
	testScheduleJoin();
}

void ThreadClassTest::testThreadClass::live(){
	while(isRunning());
}

void ThreadClassTest::testThreadClass::scheduleJoin(){
	bool *b = new bool;
	*b=true;
	ipc.send(b);
}

void ThreadClassTest::testScheduleJoin()throw(exception){
	testThreadClass threadClass;
	threadClass.start();
	threadClass.cancel();
	bool *b = threadClass.ipc.recv();
	assert_message("ipc failed",*b==true);
	delete b;
	threadClass.join();
}

}

/*
 * ThreadClassTest.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#ifndef THREADCLASSTEST_HPP_
#define THREADCLASSTEST_HPP_


#include "thread/ThreadClass.hpp"
#include "thread/IPC.hpp"
#include "testClass.hpp"

using namespace quincySystemLibrary;
namespace quincySystemLibrary_Tests {

class ThreadClassTest : public testClass {
private:
	class testThreadClass : public ThreadClass{
	private:
		void live();//entry-pt of thread.
		void scheduleJoin();//schedule another thread to call join().
	public:
		IPC<bool> ipc;
		testThreadClass(){}
		~testThreadClass(){}
	};

	//List of tests which RunTests() calls.
	void testScheduleJoin()throw(exception);
public:
	///Empty
	ThreadClassTest(){}
	///Empty
	virtual ~ThreadClassTest(){}

	virtual void RunTests()throw(exception);
};

}

#endif /* THREADCLASSTEST_HPP_ */

/*
 * ThreadTest.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#ifndef THREADTEST_HPP_
#define THREADTEST_HPP_


#include "thread/Thread.hpp"
#include "testClass.hpp"

using namespace quincySystemLibrary;
namespace quincySystemLibrary_Tests {

class ThreadTest : public testClass {
private:
	static THREADRETURN threadStarter(void *arg);

	//List of tests which RunTests() calls.
	
public:
	///Empty
	ThreadTest(){}
	///Empty
	virtual ~ThreadTest(){}

	virtual void RunTests()throw(exception);
};

}

#endif /* THREADTEST_HPP_ */

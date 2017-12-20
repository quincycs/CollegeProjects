/*
 * MutexTest.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#ifndef MUTEXTEST_HPP_
#define MUTEXTEST_HPP_


#include "thread/Mutex.hpp"
#include "testClass.hpp"

using namespace quincySystemLibrary;
namespace quincySystemLibrary_Tests {

class MutexTest : public testClass {
private:
	//List of tests which RunTests() calls.
	
public:
	///Empty
	MutexTest(){}
	///Empty
	virtual ~MutexTest(){}

	virtual void RunTests()throw(exception);
};

}

#endif /* MUTEXTEST_HPP_ */

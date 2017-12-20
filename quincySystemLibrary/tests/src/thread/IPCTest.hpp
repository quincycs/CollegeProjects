/*
 * IPCTest.hpp
 *
 *  Created on: Jul 10, 2010
 *      Author: quincy
 */

#ifndef IPCTEST_HPP_
#define IPCTEST_HPP_


#include "thread/IPC.hpp"
#include "testClass.hpp"

using namespace quincySystemLibrary;
namespace quincySystemLibrary_Tests {

class IPCTest : public testClass {
private:
	//List of tests which RunTests() calls.
	void test1()throw(exception);
public:
	///Empty
	IPCTest(){}
	///Empty
	virtual ~IPCTest(){}

	virtual void RunTests()throw(exception);
};

}

#endif /* IPCTEST_HPP_ */

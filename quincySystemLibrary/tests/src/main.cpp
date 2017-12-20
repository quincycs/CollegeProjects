//============================================================================
// Name        : tests.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "testSuite.hpp"

/*
 * tests
 */
#include "communication/sockets/SocketTCPtest.hpp"
#include "util/Line_ExceptionTest.hpp"

#include "thread/ConditionVariableTest.hpp"
#include "thread/IPCTest.hpp"
#include "thread/MutexTest.hpp"
#include "thread/ThreadClassTest.hpp"
#include "thread/ThreadTest.hpp"

using namespace quincySystemLibrary_Tests;

int main() {

	testSuite suite;

	suite.addTestClass(new Line_ExceptionTest());
	suite.addTestClass(new SocketTCPtest());
	suite.addTestClass(new ConditionVariableTest());
	suite.addTestClass(new IPCTest());
	suite.addTestClass(new MutexTest());
	suite.addTestClass(new ThreadClassTest());
	suite.addTestClass(new ThreadTest());

	suite.run();

	return 0;
}

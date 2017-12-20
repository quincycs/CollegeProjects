/*
 * SocketTCPtest.hpp
 *
 *  Created on: Jul 4, 2010
 *      Author: quincy
 */

#ifndef SOCKETTCPTEST_HPP_
#define SOCKETTCPTEST_HPP_

#include "communication/sockets/SocketTCP.hpp"
#include "testClass.hpp"
#include <vector>

using namespace quincySystemLibrary;
namespace quincySystemLibrary_Tests {

class SocketTCPtest : public testClass {

	void testServerAcceptClientConnect()throw(exception);
	void testSelect_Zero()throw(exception);
	void testAsyncSelect()throw(exception);
	void testGroupSelect()throw(exception);
public:
	SocketTCPtest();
	virtual ~SocketTCPtest();

	virtual void RunTests()throw(exception);
};

}

#endif /* SOCKETTCPTEST_HPP_ */

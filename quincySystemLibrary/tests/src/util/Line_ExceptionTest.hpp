/*
 * Line_ExceptionTest.hpp
 *
 *  Created on: Jul 4, 2010
 *      Author: quincy
 */

#ifndef LINE_EXCEPTIONTEST_HPP_
#define LINE_EXCEPTIONTEST_HPP_

#include "testClass.hpp"
#include <iostream>

namespace quincySystemLibrary_Tests {

class Line_ExceptionTest: public testClass {
private:
	void testException(int i);
	void throwException()throw(exception);
public:
	Line_ExceptionTest();
	virtual ~Line_ExceptionTest();

	virtual void RunTests()throw(exception);
};

}

#endif /* LINE_EXCEPTIONTEST_HPP_ */

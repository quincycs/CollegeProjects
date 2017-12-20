/*
 * testClass.hpp
 *
 *  Created on: Jul 4, 2010
 *      Author: quincy
 */

#ifndef TESTCLASS_HPP_
#define TESTCLASS_HPP_

#include "util/Line_Exception.hpp"
#include "util/Logger.hpp"

namespace quincySystemLibrary_Tests {

class testClass{

protected:
	testClass(){}

public:
	virtual ~testClass(){}
	/// when exceptions are thrown they do not symbolize a failed test.
	/// use: assert_message to check a condition that should fail the test.
	virtual void RunTests()throw(exception)=0;
};

}

#endif /* TESTCLASS_HPP_ */

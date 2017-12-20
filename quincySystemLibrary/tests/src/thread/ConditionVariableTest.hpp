/*
 * ConditionVariableTest.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#ifndef CONDITIONVARIABLETEST_HPP_
#define CONDITIONVARIABLETEST_HPP_

#include "thread/ConditionVariable.hpp"
#include "testClass.hpp"

using namespace quincySystemLibrary;
namespace quincySystemLibrary_Tests {

class ConditionVariableTest : public testClass {
public:
	ConditionVariableTest(){}
	virtual ~ConditionVariableTest(){}

	virtual void RunTests()throw(exception);
};

}

#endif /* CONDITIONVARIABLETEST_HPP_ */

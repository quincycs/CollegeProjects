/*
 * ConditionVariable.hpp
 *
 *  Created on: Jul 8, 2010
 *      Author: quincy
 */

#ifndef CONDITIONVARIABLE_HPP_
#define CONDITIONVARIABLE_HPP_

#include "thread/posix/ConditionVariableImp.hpp"

namespace quincySystemLibrary {

class Mutex;

class ConditionVariable : private ConditionVariableImp {
public:
	ConditionVariable(Mutex &mutex):ConditionVariableImp(mutex){}
	virtual ~ConditionVariable(){}

	void signal(){ConditionVariableImp::signal();}
	void wait(){ConditionVariableImp::wait();}
};

}

#endif /* CONDITIONVARIABLE_HPP_ */

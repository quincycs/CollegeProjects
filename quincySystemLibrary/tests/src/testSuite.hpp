/*
 * testSuite.hpp
 *
 *  Created on: Jul 4, 2010
 *      Author: quincy
 */

#ifndef TESTSUITE_HPP_
#define TESTSUITE_HPP_

#include "testClass.hpp"
#include <queue>
#include <iostream>

namespace quincySystemLibrary_Tests {

class testSuite {
private:
	std::queue<testClass*> q;
public:
	testSuite(){}
	~testSuite(){}

	///add dynamically allocated test. (will be freed after ran)
	void addTestClass(testClass *test){q.push(test);}

	void run(){
		while(!q.empty() )
		{
			testClass *test = q.front();

			try{
				test->RunTests();
			}catch(exception e){
				std::cerr << e.what()<<std::endl;
			}

			std::cerr << ".";
			delete test;
			q.pop();
		}
		std::cerr << "TestSuite Passed." << std::endl;
	}
};

}

#endif /* TESTSUITE_HPP_ */

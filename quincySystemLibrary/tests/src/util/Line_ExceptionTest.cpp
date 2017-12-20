/*
 * Line_ExceptionTest.cpp
 *
 *  Created on: Jul 4, 2010
 *      Author: quincy
 */

#include "Line_ExceptionTest.hpp"

namespace quincySystemLibrary_Tests {

Line_ExceptionTest::Line_ExceptionTest() {
	// TODO Auto-generated constructor stub

}

Line_ExceptionTest::~Line_ExceptionTest() {
	// TODO Auto-generated destructor stub
}

void Line_ExceptionTest::throwException()throw(exception){
	throw Line_Exception("run time error");
}

void Line_ExceptionTest::testException(int i){

try{
	if(i==5) 	throwException();
	else 		testException(i+1);
}catch(exception &e){
	assert_message("exception caught does not have the same message",
			strcmp(e.what(),"\nException thrown: run time error\nLocation:: Line 22 of ../src/util/Line_ExceptionTest.cpp\n")==0);
	if(i!=1)throw;
}
catch(...){
	assert_message("catching different type of exception than thrown",false);
}
}

void Line_ExceptionTest::RunTests()throw(exception){

	testException(1);
}

}

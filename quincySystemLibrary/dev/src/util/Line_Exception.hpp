/*
 * RJS_Exception.h
 *
 *  Created on: Jun 12, 2010
 *      Author: quincy
 */

#ifndef RJS_EXCEPTION_H_
#define RJS_EXCEPTION_H_

#include <string>
using std::string;
#include <exception>
using std::exception;
#include <cassert>
#include <sstream>

#define Line_Exception(message)		\
	::quincySystemLibrary::_Line_Exception(message, __FILE__, __LINE__, __ASSERT_FUNCTION)

namespace quincySystemLibrary {

class _Line_Exception : public exception {
private:
	string errorString;
public:
	_Line_Exception(string message, const char *file,
			   unsigned int line, const char *function) throw()
		{
			std::stringstream ss;
			ss << line;
			this->errorString=
				"\nException thrown: " + message +
				"\nLocation:: Line " + ss.str() + " of " + file + "\n";
		}
	virtual ~_Line_Exception() throw() {}


	virtual const char* what() const throw() { return errorString.c_str(); }
};

}



#endif /* RJS_EXCEPTION_H_ */

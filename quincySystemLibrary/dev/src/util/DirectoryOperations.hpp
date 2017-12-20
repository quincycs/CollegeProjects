/*
 * MkDir.hpp
 *
 *  Created on: Jun 7, 2010
 *      Author: quincy
 */

#ifndef DIRECTORYOPERATIONS_HPP_
#define DIRECTORYOPERATIONS_HPP_

#include <string>
using std::string;
#include <stdio.h>
#include "util/Line_Exception.hpp"

namespace quincySystemLibrary {

/// Abstraction to create, erase directories and set the current working directory.  Supports relative paths and full paths.
class DirectoryOperations {

public:

	///throws exception iff returns false.
	static bool SetCurrentDirectory(string path) throw(exception);
	static bool Create(string path);
	static bool Erase(string path)throw(exception);
};

}

#endif /* DIRECTORYOPERATIONS_HPP_ */

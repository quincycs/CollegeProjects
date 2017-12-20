/*
 * MkDir.cpp
 *
 *  Created on: Jun 7, 2010
 *      Author: quincy
 */

#include "DirectoryOperations.hpp"

namespace quincySystemLibrary {


bool DirectoryOperations::SetCurrentDirectory(string path) throw(exception){

	if(chdir(path.c_str())==0){
		return true;
	}

	throw Line_Exception("could not change current directory to "+path);
	return false;
}

bool DirectoryOperations::Erase(string path)throw(exception){

	if(0==remove(path.c_str())){
		return true;
	}

	throw Line_Exception("could not erase directory: "+path);
	return false;
}

bool DirectoryOperations::Create(string path){

	string mkdir_cmd = "mkdir -p "+path;

	FILE *pipe = popen(mkdir_cmd.c_str(),"r");

	int exitcode = pclose(pipe);

	return (exitcode == 0);
}


}

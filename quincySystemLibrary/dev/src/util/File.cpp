/*
 * File.cpp
 *
 *  Created on: Jun 7, 2010
 *      Author: quincy
 */

#include "File.hpp"

namespace quincySystemLibrary {

File::File(string filename) {
	this->filename = filename;
	file=NULL;
	_gcount=0;
}

File::~File() {
	close();
}

void File::close(){
	if(file!=NULL ){
		fclose(file);
	}
	file=NULL;
}

bool File::isExisting(){

	struct stat statBuf;
	int ret = stat(filename.c_str(), &statBuf);
	if(ret==-1)return false;

	return true;
}

bool File::create(){
	file = fopen(filename.c_str(),"w");
	bool b=(file!=NULL);
	close();

	return b;
}

bool File::openForOnlyReading(){
	file = fopen(filename.c_str(),"r");

	return file!=NULL;
}
bool File::openForOnlyAppending(){
	file = fopen(filename.c_str(),"a");

	return file!=NULL;
}

bool File::read(void *data, uint32_t data_size){
	_gcount = fread(data,sizeof(char),data_size,file);
	return _gcount != 0;
}

void File::write(const void *data, uint32_t data_size){
	_gcount = fwrite(data,sizeof(char), data_size, file);
	fflush(file);
}

uint32_t File::getCheckSum(){


	uint32_t data_size = 1048576;//MB
	char *data = new char[data_size];

	boost::crc_32_type crc;

	while(read(data,data_size)){
		crc.process_bytes(data,gcount());
	}

	return crc.checksum();
}

bool File::erase(){
	///remove from stdio.h
	return 0==remove(filename.c_str());
}

bool File::untar() throw(exception*){

	string untar_cmd = "tar -xzf "+filename;

	FILE *pipe = popen(untar_cmd.c_str(),"r");

	int exitcode = pclose(pipe);

	if(exitcode == 0){
		return true;
	}

	throw Line_Exception("could not untar "+filename);
	return false;
}

}

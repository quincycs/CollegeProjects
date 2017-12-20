/*
 * RJSLogger.cpp
 *
 *  Created on: Apr 12, 2010
 *      Author: quincy
 */

#include "Logger.hpp"

#include <iostream>
using std::cerr;
using std::endl;

using namespace boost::interprocess;

namespace quincySystemLibrary {

Logger *Logger::log = 0;

Logger::Logger() {

}

Logger::~Logger() {

}

bool Logger::Initialize(char *filename, uint32_t logLevel){

	if(logLevel==1 || filename==NULL){
		log = NULL;
		return true;
	}

	log = new Logger();
	log->file = new File(string(filename));

	if( ! log->file->create() ){
		Destroy();
		return false;
	}

	log->file->openForOnlyAppending();
	Write("Started");

	return true;
}

void Logger::Destroy(){
	if(log==NULL)return;

	if(log->file != NULL){
		delete log->file;
		log->file=NULL;
	}

	delete log;
	log=NULL;
}

void Logger::Write(string message){

	message=message+"\n";

	if(log!=NULL && log->file!=NULL){
		log->mux.lock();
		log->file->write(message.c_str(), message.length());
		log->mux.unlock();
	}else{
		cerr << message;
	}


}



}

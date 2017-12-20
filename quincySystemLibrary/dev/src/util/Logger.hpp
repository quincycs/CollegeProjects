/*
 * RJSLogger.hpp
 *
 *  Created on: Apr 12, 2010
 *      Author: quincy
 */

#ifndef RJSLOGGER_HPP_
#define RJSLOGGER_HPP_

/*
 * If logLevel set to more than 1 then this log reports everything
 */


#include <string>
using std::string;
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include "util/File.hpp"

#include <assert.h>

#include <sstream>


namespace quincySystemLibrary {

///Log messages into a file or stderr for debugging. Provides specialized assert functions.
class Logger {
private:
	Logger();
	virtual ~Logger();

	static Logger *log;

	File *file;
	uint32_t logLevel;

	boost::interprocess::interprocess_mutex mux;
public:
	/// if logLevel > 1 and filename is not NULL symbolizes that the logfile wants to be created.
	/// if logfile does not want to be created then all messages are put on stderr
	/// Returns true if logfile was created and wanted to be created.
	static bool Initialize(char *filename, uint32_t logLevel);
	/// Deletes file if it was constructed.
	static void Destroy();

	static void Write(string message);
	/// Always put in the define: AT into the first argument.
	static void Write(string at, string message){
		Write("Location:"+at+"\n"+message);
	}
	/// Always put in the condition you want to be true.
	static void Write_If(string message, bool condition){

		if(!condition){
			Write(message);
		}
	}

	/*
	__assert_fail (__const char *__assertion, __const char *__file,
				   unsigned int __line, __const char *__function)
	*/
	/// Always put in the define: AT into the first argument, and put in the condition you want to be true.
	static void AssertFailed(string message, const char *assertion, const char *file,
			   unsigned int line, const char *function){

		std::stringstream ss;
		ss << line;
		Write(string("HARD ASSERT FAIL!!\n")+message+": "+assertion+
				"\nLocation:: Line "+ss.str()+" of "+file+"\n");
		exit(EXIT_FAILURE);
	}

};

}

#undef assert_message
#define assert_message(message,expr)							\
  ((expr)								\
   ? __ASSERT_VOID_CAST (0)						\
   : quincySystemLibrary_AssertFailed (message,__STRING(expr), __FILE__, __LINE__, __ASSERT_FUNCTION))

inline void quincySystemLibrary_AssertFailed(string message, const char *assertion, const char *file,
			   unsigned int line, const char *function){

	quincySystemLibrary::Logger::AssertFailed(message,assertion,file,line,function);
}


#endif /* RJSLOGGER_HPP_ */

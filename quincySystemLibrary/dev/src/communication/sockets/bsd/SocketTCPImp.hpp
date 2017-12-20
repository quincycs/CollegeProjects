

#ifndef _SOCKETTCPPOSIX_H_
#define _SOCKETTCPPOSIX_H_

#ifdef USE_BSD_SOCKETS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>//C strings
#include <string>//C++ strings
using std::string;
#ifndef MINGW
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <sys/resource.h>
#include <vector>
#else
#include <winsock2.h>
#include <process.h>
#endif

/*
 * i included these.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "util/Line_Exception.hpp"

using namespace std;

namespace quincySystemLibrary {
/**
 * This socket class provides an interface for POSIX socket communications.
 *
 */
class SocketTCP;

class SocketTCPImp {
private:

	/// Stores the socket file descriptor for the socket instance.
	int _socket;

	/// Stores information about the destination computer/connection
	struct sockaddr_in fromAddr;

	/// This checks for the right version of libraries in windows.
	/// @return True if the correct libraries are in place, false if not.
	bool InitializeSocketLibrary() throw(exception);

	/// This stores error messages when they are generated during the execution of the socket.
	string errorString;
	void setErrorString(string errorString_arg);

	/// returns true if Close() was called.  Socket is closed if an system call is returned with an unrecoverable error.
	/// use this function to test whether an error happened. any error is considered an unrecoverable failure,
	/// due to this socket class using a blocking socket none of the system calls should return an error unless,
	/// there was a connectivity failure.
	bool IsClosed();

	static int GroupSelect(uint32_t count, fd_set &fdset, uint32_t millisec_timeout) throw(exception);

	/// Adds the socket fd to the fd set.
	/// @param[in] fdset A file descriptor set of sockets that the socket fd should be added to.
	/// returns Socket pointer (to be used to determine the largest socket pointer, which is needed by select)
	int AddToSelectSet(fd_set &fdset);

	/// Determines if the socket fd is in the current set.
	/// @param[in] fdset The file descriptor set that should be looked in for the socket fd.
	/// @returns Returns true if the file descriptor is in the fd set.
	bool IsInSelectSet(fd_set &fdset);

protected:
	SocketTCPImp(){}
	/// Calls InitializeSocketLibrary() and attempts to create a new socket file descriptor if openFD is true.
	/// @param[in] openFD If true, the constructor will open a new socket file descriptor.
	///				(for example, pass a false if you are just going to pass it into Accept)
	//SocketTCPImp(bool openFD = true) throw(exception*);

	/// Calls Close().
	virtual ~SocketTCPImp();

	/// Accepts a connection from a client
	/// @param[out] retSocket Must contain a socket instance with an unopened socket file descriptor (this function won't close the socket FD if it is open, but can't reuse either).
	bool accept(SocketTCPImp *retSocket) throw(exception);

	/// Binds the socket to the provided port. Constructs underlying socket.
	/// @param[in] port The port number to bind to.
	/// @return Returns true if successful.
	bool bind(uint32_t port) throw(exception);

	/// Closes the socket file descriptor of the instance
	/// @return Returns true if successful.
	void close() throw(exception);

	/// returns hostname of destination in IPv4 decimal notation
	string getHostName();

	/// Connects to another computer's port as a client. Constructs underlying socket.
	/// @param[in] hostname The DNS resolvable name of the destination.
	/// @param[in] port The port number to connect to.
	/// @returns Returns true if successfully connected, false otherwise.
	bool connect(string hostname, uint32_t port) throw(exception);

	/// Listens on its port for incoming connection requests.
	/// @param[in] queueSize The number of incoming connection requests it can queue.
	/// @returns Returns false if errors occurred.
	bool listen(uint32_t queueSize) throw(exception);

	/// Receives data on the socket.
	/// ****CAREFUL***** this does not null terminate buffer
	/// @param[out] buffer The buffer to place the received data in.
	/// @param[in] desired_len The max size to receive. This length is not guaranteed to be received, this is only a limit.
	/// @returns Returns the number of bytes read from the socket, thrown exception on error.
	uint32_t recvNonBlock(char *buffer, uint32_t desired_len) throw(exception);

	/// Receives a specific amount of data from the socket.
	/// ****CAREFUL***** this does not null terminate buffer
	/// @param[out] buffer The buffer to place the received data in.
	/// @param[in] desired_len The amount of data that should be (live) waited for. Will wait until this much data arrives or a read error occurs.
	/// @returns Returns the number of bytes read from the socket, thrown exception on error.
	uint32_t recvBlock(char *buffer, uint32_t desired_len) throw(exception);

	/// Sends data on the socket.  Warning, if recv'er closes while we are sending, SIGPIPE is thrown. Default action of SIGPIPE is to terminate.
	/// @param[in] str The buffer containing the data to be sent.
	/// @param[in] strlen The length of the buffer from the first argument.
	/// @returns Returns the number of bytes written to the socket. Returns -1 if an error occurred.
	int sendBlock(const char *str, uint32_t strlen) throw(exception);

	/// Blocks until something is ready to be read from the socket or something is in the cache, or the timeout has expired.
	/// @param[in] millisec_timeout The length of time to wait for activity on the socket (in milliseconds).
	/// returns Returns true if there is something waiting to be read from the socket. Returns false if the timeout expired.
	bool select(uint32_t millisec_timeout) throw(exception);

	static vector<SocketTCP*> groupSelect(vector<SocketTCPImp*> &socketList, uint32_t millisec_timeout);

	/// Gets the error string for the socket
	/// returns Returns the error string.
	const char *GetErrorString();
};

/*
 *  INLINE FUNCTIONS
 */


inline bool SocketTCPImp::IsClosed(){

	return _socket==-1;
}

inline bool SocketTCPImp::IsInSelectSet(fd_set &fdset) {
	return FD_ISSET(_socket, &fdset);
}

inline int SocketTCPImp::AddToSelectSet(fd_set &fdset) {
	FD_SET(_socket, &fdset);
	return _socket;
}


inline const char * SocketTCPImp::GetErrorString() {
	return errorString.c_str();
}


}

#endif
#endif

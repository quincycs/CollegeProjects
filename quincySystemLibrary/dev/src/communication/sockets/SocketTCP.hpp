/*
 * Socket.hpp
 *
 *  Created on: Jul 3, 2010
 *      Author: quincy
 */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include "util/Line_Exception.hpp"
#include "bsd/SocketTCPImp.hpp"		//USE_BSD_SOCKETS
#include "boost/SocketTCPImp.hpp"	//USE_BOOST_SOCKETS

#include <vector>
using std::vector;


namespace quincySystemLibrary {

/**
 * Interface into socket implementations.  Each method throws an exception when an unexpected socket error occurs such as the socket being closed by the peer.
 *              Current library implementation choices are:
 *                      boost::asio     to use this implementation, build library with the define: USE_BOOST_SOCKETS
 *                      BSD             USE_BSD_SOCKETS
 */
class SocketTCP : private SocketTCPImp {
public:
	SocketTCP(){}
	/// Implementations close the underlying socket if open.
	virtual ~SocketTCP(){}
	/// Accepts a new pending connection.
	void accept(SocketTCP *sock)throw(exception){SocketTCPImp::accept(sock);}
	/// Initialize connection to a local port.
	void bind(uint32_t port)throw(exception){SocketTCPImp::bind(port);}
	/// Initialize queue and set the max amount of pending connections.  A connection that can't fit in the queue is dropped.
	void listen(uint32_t queueSize)throw(exception){SocketTCPImp::listen(queueSize);}
	/// Close the connection.
	void close()throw(exception){SocketTCPImp::close();}
	/// Get the host name of the peer.
	string getHostName()throw(exception){return SocketTCPImp::getHostName();}
	/// Connect to a server which is accepting new connections on a specific port.
	void connect(string hostname, uint32_t port)throw(exception){SocketTCPImp::connect(hostname,port);}
	/// Non-Blocking receives some data on socket.  Does not read more than len.
	/// WARNING: does not null terminate buffer
	/// returns number of bytes actually read.
	uint32_t recvNonBlock(char *buffer, uint32_t len)throw(exception){return SocketTCPImp::recvNonBlock(buffer,len);}
	/// Blocking receives exactly desired_len amount of data
	/// WARNING: does not null terminate buffer
	void recvBlock(char *buffer, uint32_t desired_len)throw(exception){SocketTCPImp::recvBlock(buffer,desired_len);}
	/// Blocking send.
	/// WARNING: if using POSIX implementation an error on this may signal a SIGPIPE which by default on POSIX systems terminate the process.
	void sendBlock(const char *buffer, uint32_t len)throw(exception){SocketTCPImp::sendBlock(buffer,len);}
	/// Blocks for [ms] until data is ready to be read on socket.
	/// returns true only if data is ready to be read on socket.
	bool select(uint32_t ms_timeout)throw(exception){return SocketTCPImp::select(ms_timeout);}
	/// Blocks for [ms] until data is ready to be read on at least one socket.
	/// returns the list of ready to read sockets.
	static vector<SocketTCP*> groupSelect(vector<SocketTCP*> &sockList, uint32_t ms_timeout)
			{return SocketTCPImp::groupSelect((vector<SocketTCPImp*>&)sockList,ms_timeout);}
};

}

#endif /* SOCKET_HPP_ */

/*
 * SocketBoost.hpp
 *
 *  Created on: Jul 3, 2010
 *      Author: quincy
 */

#ifndef SOCKETBOOST_HPP_
#define SOCKETBOOST_HPP_

#ifdef USE_BOOST_SOCKETS
#include <boost/asio.hpp>
#include <boost/timer.hpp>
#include <vector>
using std::vector;
#include "util/Line_Exception.hpp"
#include <sstream>
#include <time.h>

using namespace boost::asio::ip;
namespace quincySystemLibrary {

class SocketTCP;
/**
 * TCP socket class implementation from boost.
 */
class SocketTCPImp {
private:
	///underlying structure which is used for sending and receiving data.
	tcp::socket *sock;
	///underlying structure which is used to accept connection.
	tcp::acceptor *acceptor;
	boost::asio::io_service io_serve;
	tcp::endpoint endpoint;
public:

	SocketTCPImp();
	/// close the underlying socket.
	virtual ~SocketTCPImp();
	/// After method returns sock is ready to be read.
	void accept(SocketTCPImp *sock)throw(exception);
	void bind(uint32_t port)throw(exception);
	void close()throw(exception);
	string getHostName()throw(exception);
	void connect(string hostname, uint32_t port)throw(exception);
	void listen(uint32_t queueSize)throw(exception);
	/// Non-Blocking receives some data on socket.  Does not read more than len.
	/// does not null terminate buffer
	/// returns number of bytes actually read.
	/// throws exception when socket is closed by peer or when some error happens which closes the socket.
	uint32_t recvNonBlock(char *buffer, uint32_t len)throw(exception);
	/// Blocking receives exactly desired_len amount of data
	/// does not null terminate buffer
	/// throws exception when socket is closed by peer or when some error happens which closes the socket.
	void recvBlock(char *buffer, uint32_t desired_len)throw(exception);
	/// Blocking send.
	/// throws exception when socket is closed by peer or when some error happens which closes the socket.
	/// WARNING: if using POSIX implementation an error on this may signal a SIGPIPE which by default on POSIX systems terminate the process.
	void sendBlock(const char *buffer, uint32_t len)throw(exception);
	/// CAREFUL: this implementation has very bad performance.  Boost::asio does not support select().  This implementation spins on a method which checks if there are any new bytes available to be read.  After every failed check the thread goes to sleep for a millisecond.
	/// Blocks for [ms] until data is ready to be read on socket.
	/// returns true only if data is ready to be read on socket.
	bool select(uint32_t ms_timeout)throw(exception);
	/// CAREFUL: this implementation has very bad performance.  Boost::asio does not support select().  This implementation spins on a method which checks if there are any new bytes available to be read.  After every failed check the thread goes to sleep for a millisecond.
	/// Blocks for [ms] until data is ready to be read on at least one socket.
	/// returns list of sockets which are ready to be read.
	static vector<SocketTCP*> groupSelect(vector<SocketTCPImp*> &sockList, uint32_t ms_timeout);
};

}

#endif
#endif /* SOCKETBOOST_HPP_ */

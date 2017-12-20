/*
 * SocketBoost.cpp
 *
 *  Created on: Jul 3, 2010
 *      Author: quincy
 */

#include "SocketTCPImp.hpp"

#ifdef USE_BOOST_SOCKETS

namespace quincySystemLibrary {

SocketTCPImp::SocketTCPImp() {
	sock=NULL;
	acceptor=NULL;
}

SocketTCPImp::~SocketTCPImp() {
	if(sock!=NULL)delete sock;
	if(acceptor!=NULL)delete acceptor;
}

void SocketTCPImp::accept(SocketTCPImp *sockTCP)throw(exception){
	sockTCP->sock = new tcp::socket(sockTCP->io_serve);
	acceptor->accept(*(sockTCP->sock),endpoint);
}

void SocketTCPImp::bind(uint32_t port)throw(exception){

	tcp::resolver resolver(io_serve);
	std::stringstream ss; ss << port;
	tcp::resolver::query query("127.0.0.1", ss.str());
	endpoint = *resolver.resolve(query);
	acceptor = new tcp::acceptor(io_serve);
	acceptor->open(endpoint.protocol());
	acceptor->set_option(tcp::acceptor::reuse_address(true));
	acceptor->bind(endpoint);
}

void SocketTCPImp::close()throw(exception){
	sock->close();
}

string SocketTCPImp::getHostName()throw(exception){
	address add = sock->remote_endpoint().address();
	return add.to_string();
}

void SocketTCPImp::connect(string hostname, uint32_t port)throw(exception){

	sock = new tcp::socket(io_serve);

	// Get a list of endpoints corresponding to the server name.
	tcp::resolver resolver(io_serve);
	std::stringstream ss; ss << port;
	tcp::resolver::query query(hostname, ss.str());
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;

	// Try each endpoint until we successfully establish a connection.
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end)
	{
	  sock->close();
	  sock->connect(*endpoint_iterator++, error);
	}
	if (error)
	  throw boost::system::system_error(error);
}

void SocketTCPImp::listen(uint32_t queueSize)throw(exception){
	acceptor->listen(queueSize);
}
/// Non-Blocking receives some data on socket.  Does not read more than len.
/// does not null terminate buffer
/// returns number of bytes actually read.
/// throws exception when socket is closed by peer or when some error happens which closes the socket.
uint32_t SocketTCPImp::recvNonBlock(char *data, uint32_t len)throw(exception){
	return sock->read_some(boost::asio::buffer(data,len));
}
/// Blocking receives exactly desired_len amount of data
/// does not null terminate buffer
/// throws exception when socket is closed by peer or when some error happens which closes the socket.
void SocketTCPImp::recvBlock(char *data, uint32_t desired_len)throw(exception){
	boost::asio::read(*sock, boost::asio::buffer(data,desired_len),
						boost::asio::transfer_at_least(desired_len));
}
/// Blocking send.
/// throws exception when socket is closed by peer or when some error happens which closes the socket.
/// WARNING: if using POSIX implementation an error on this may signal a SIGPIPE which by default on POSIX systems terminate the process.
void SocketTCPImp::sendBlock(const char *buffer, uint32_t len)throw(exception){

	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream.write(buffer,len);
	boost::asio::write(*sock, request);
}
/// Blocks for [ms] until data is ready to be read on socket.
/// returns true only if data is ready to be read on socket.
/// throws exception when some socket error closes the socket
bool SocketTCPImp::select(uint32_t ms_timeout)throw(exception){

	double sec_timeout = double(ms_timeout/1000);
	bool avail;
	boost::timer time;
	while(!(avail=sock->available()>0) && time.elapsed() < sec_timeout ){
		struct timespec rm;
		rm.tv_sec=0;
		rm.tv_nsec=1000000;
		nanosleep(&rm,NULL);
	}

	return avail;
}

vector<SocketTCP*> SocketTCPImp::groupSelect(vector<SocketTCPImp*> &sockList, uint32_t ms_timeout){

	vector<SocketTCP*> selectedSockets;
	double sec_timeout = double(ms_timeout/1000);

	boost::timer time;

	do{
		vector<SocketTCPImp*>::iterator iter = sockList.begin();
		for(; iter!=sockList.end(); ++iter){
			if((*iter)->sock->available()){
				selectedSockets.push_back((SocketTCP*)(*iter));
			}
		}

		if(!selectedSockets.empty()){
			return selectedSockets;
		}

		struct timespec rm;
		rm.tv_sec=0;
		rm.tv_nsec=1000000;
		nanosleep(&rm,NULL);

	}while( time.elapsed() < sec_timeout );

	return selectedSockets;
}

}

#endif

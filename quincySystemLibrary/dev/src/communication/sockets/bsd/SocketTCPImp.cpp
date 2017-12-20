
#ifdef USE_BSD_SOCKETS

#include "SocketTCPImp.hpp"

namespace quincySystemLibrary {

void SocketTCPImp::setErrorString(string errorString_arg){

	errorString = errorString_arg;
	errorString.append(" :: ");
	errorString.append(strerror(errno));
}

bool SocketTCPImp::accept(SocketTCPImp *retSocket) throw(exception){
	struct sockaddr_in fromAddr;
#ifdef MINGW
	int len;
#else
	socklen_t len;
#endif
	len = sizeof(fromAddr);
	int socket;
	bool ret = true;
	if ((socket = accept(_socket, (struct sockaddr*) &fromAddr, &len)) < 0) {

		setErrorString("Socket: error in accept.");
		throw Line_Exception(errorString);

		ret = false;
	}
	retSocket->_socket = socket;
	retSocket->fromAddr.sin_addr=fromAddr.sin_addr;

	return ret;
}

bool SocketTCPImp::bind(uint32_t port) throw(exception){

	InitializeSocketLibrary();
	if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //creates blocking TCP socket
		setErrorString("error in socket creation.");
		throw Line_Exception(errorString);
	}

	int intOne=1;
	setsockopt(_socket,SOL_SOCKET,SO_REUSEADDR,&intOne,sizeof(int));

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	if (bind(_socket, (struct sockaddr*) &sin, sizeof(sin)) < 0) {

		setErrorString("Socket: error in bind.");
		throw Line_Exception(errorString);

		return false;
	} else
		return true;
}

void SocketTCPImp::close() throw(exception){
	if (_socket != -1) {
		if (shutdown(_socket, 2) < 0) {

			setErrorString("Socket: error in shutdown.");
			throw Line_Exception(errorString);
		}
		close(_socket);
		_socket = -1;
	}
}

string SocketTCPImp::getHostName(){
	return string(inet_ntoa(fromAddr.sin_addr));
}

bool SocketTCPImp::connect(string hostname, uint32_t port) throw(exception){

	InitializeSocketLibrary();
	if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //creates blocking TCP socket
		setErrorString("error in socket creation.");
		throw Line_Exception(errorString);
	}

	struct sockaddr_in sin;
	struct hostent *hp;
	hp = gethostbyname(hostname.c_str());
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	memcpy((char *) &sin.sin_addr, (char *) hp->h_addr, hp->h_length);

	if ((connect(_socket, (struct sockaddr*) &sin, sizeof(sin))) < 0) {
		setErrorString("Socket: error in connect.");
		throw Line_Exception(errorString);

		return false;
	} else
		return true;
}

bool SocketTCPImp::listen(uint32_t queueSize) throw(exception){
	int e;
	if ((e = listen(_socket, queueSize)) < 0) {
		setErrorString("Socket: error in listen.");
		throw Line_Exception(errorString);

		return false;
	} else
		return true;
}

int SocketTCPImp::sendBlock(const char *str, uint32_t strlen) throw(exception){
	// WIN vs UNIX
#ifdef MINGW
	int n=send(_socket,str,strlen,0);
	if (n==SOCKET_ERROR)
		n=-1;
#else
	int n = write(_socket, str, strlen);
#endif

	if(n==-1){

		setErrorString("Socket: error in send.");
		close();
		throw Line_Exception(errorString);
	}

	return n;
}


uint32_t SocketTCPImp::recvNonBlock(char *buffer, uint32_t buf_len) throw(exception){
	// WIN vs UNIX
	uint32_t len = 0;
	int n=0;

	if(len < buf_len && select(10000) )
	{

#ifdef MINGW
		n=recv(_socket,buffer+len,buf_len-len,0);
#else
		n = read(_socket, buffer+len, buf_len-len);
#endif
		if(n<=0){
			if(n==0) setErrorString("Socket: closed by peer.");
			if(n<0)	setErrorString("Socket: error in recvBlock. Socket now closed.");
			close();

			throw Line_Exception(errorString);
			return 0;
		}

		// n > 0
		len=len+n;


	}

	return len;
}

uint32_t SocketTCPImp::recvBlock(char *buffer, uint32_t desired_len) throw(exception){
	uint32_t len = 0 ;
	int n = 0;

	while (len < desired_len && ! IsClosed() ) {

		if(select(10000)){
#ifdef MINGW
		n=recv(_socket,buffer+len,desired_len-len,0);

#else
		n = read(_socket, buffer + len, desired_len - len);
#endif

		if(n<=0){

			if(n==0) setErrorString("Socket: closed by peer.");
			if(n<0)	setErrorString("Socket: error in recvBlock. Socket now closed.");

			close();
			throw Line_Exception(errorString);
			return 0;
		}

		// n > 0
		len += n;
	}}

	return len;
}

//static
bool SocketTCPImp::select(uint32_t millisec_timeout)throw(exception) {

	if(IsClosed())return false;//give a false, if socket was closed instead of segFault.

	fd_set fdset;
	//Initialize set
	FD_ZERO(&fdset);
	//Add the socket fd to the set
	FD_SET(_socket,&fdset);
	struct timeval tv;
	tv.tv_sec = millisec_timeout / 1000; // set the seconds
	tv.tv_usec = (millisec_timeout - tv.tv_sec * 1000) * 1000; // set the microseconds
	if (select(_socket + 1, &fdset, NULL, NULL, &tv) < 0) {
		setErrorString("Socket: error in select.");
		throw Line_Exception(errorString);
		return false;
	}
	return FD_ISSET(_socket,&fdset);
}

int SocketTCPImp::GroupSelect(uint32_t count, fd_set & fdset, uint32_t millisec_timeout) throw(exception){
	struct timeval tv;
	int numReady = 0;
	tv.tv_sec = millisec_timeout % 1000; // set the seconds
	tv.tv_usec = (millisec_timeout - tv.tv_sec * 1000) * 1000; // set the microseconds
	if ((numReady = select(count, &fdset, NULL, NULL, &tv)) < 0) {

		throw Line_Exception("Socket: error in GroupSelect.");
	}
	return numReady;
}


//static
vector<SocketTCP*> SocketTCPImp::groupSelect(vector<SocketTCPImp*> &socketList, uint32_t millisec_timeout){

	/*
	 * add sockets into fdset and toTrySelect to be group selected only if they don't have cached contents.
	 *
	 * selectedSockets is the result of this function, so add sockets with cached contents into final output.
	 */
	vector<SocketTCP*> selectedSockets;
	vector<SocketTCPImp*> toTrySelect;
	fd_set fdset;
	FD_ZERO(&fdset);

	int largestSocketPointer=-1;
	int temp;
	vector<SocketTCPImp*>::iterator sockIter = socketList.begin();
	for(; sockIter != socketList.end(); ++sockIter){

		if( (*sockIter)->IsClosed() )continue;

		toTrySelect.push_back(*sockIter);

		temp=(*sockIter)->AddToSelectSet(fdset);
		if(largestSocketPointer < temp) largestSocketPointer=temp;

	}

	int numReady=SocketTCPImp::GroupSelect(largestSocketPointer + 1, fdset, millisec_timeout);

	if(numReady <= 0 && selectedSockets.empty() ){//very common so lets make it a special case.
		vector<SocketTCP*> emptyList;
		return emptyList;
	}

	/*
	 * put sockets in fdset into selectedSockets vector
	 */

	for( sockIter = toTrySelect.begin(); sockIter != toTrySelect.end(); ++sockIter ){

		if((*sockIter)->IsInSelectSet(fdset)){
			selectedSockets.push_back((SocketTCP*)*sockIter);
		}

	}

	return selectedSockets;

}

bool SocketTCPImp::InitializeSocketLibrary() throw(exception){
#ifdef MINGW
	static bool alreadyInitialized=false;
	if (! alreadyInitialized) {
		alreadyInitialized=true;
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;
		int hi=1,lo=1;
		wVersionRequested = MAKEWORD( hi,lo );

		err = WSAStartup( wVersionRequested, &wsaData );
		if ( err != 0 ) {
			// Tell the user that we could not find a usable
			// WinSock DLL.
			setErrorString("Failure in enabling socket library.");
			throw new Line_Exception(AT, errorString);


			return false;
		}

		// check the returned hi and low version numbers
		int retlo, rethi;
		retlo=LOBYTE( wsaData.wVersion );
		rethi=HIBYTE( wsaData.wVersion );
		if ( retlo!= lo || rethi != hi ) {
			// Tell the user that we could not find a usable
			// WinSock DLL.
			WSACleanup( );

			errorString.assign("Requested bad version of WinSock. Request for version ");
			char hiloStr[50];
			sprintf(hiloStr,"%i.%i", hi,lo);
			errorString.append(hiloStr);

			errorString.append(" failed. Received version ");
			char rethiloStr[50];
			sprintf(rethiloStr,"%i.%i",rethi,retlo);
			errorString.append(rethiloStr);
			errorString.append(" instead.");

			throw Line_Exception(AT, errorString);

			return false;
		}
	}
#endif
	return true;
}

SocketTCPImp::~SocketTCPImp() {
	close();
}


}

#endif

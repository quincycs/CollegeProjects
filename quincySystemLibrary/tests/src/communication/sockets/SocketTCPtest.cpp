/*
 * SocketTCPtest.cpp
 *
 *  Created on: Jul 4, 2010
 *      Author: quincy
 */

#include "SocketTCPtest.hpp"
#include <iostream>
using std::cerr;using std::endl;

namespace quincySystemLibrary_Tests {

SocketTCPtest::SocketTCPtest() {
	// TODO Auto-generated constructor stub

}

SocketTCPtest::~SocketTCPtest() {
	// TODO Auto-generated destructor stub
}

void SocketTCPtest::testServerAcceptClientConnect()throw(exception){

	/*
	 * testing server accept and read
	 *  and client connect and send.
	 */
	SocketTCP server;

	server.bind(11000);
	server.listen(10);

	SocketTCP sender;
	sender.connect("127.0.0.1",11000);

	SocketTCP reader;
	server.accept(&reader);

	string senderString = "one two three.";
	sender.sendBlock(senderString.c_str(),senderString.length());

	char receivedData[senderString.length()+1];
	receivedData[senderString.length()]='\0';
	reader.recvBlock(receivedData,senderString.length());

	assert_message("The strings didn't match!",senderString==string(receivedData));
}

void SocketTCPtest::testSelect_Zero()throw(exception){
	SocketTCP server;
	server.bind(11000);
	server.listen(10);

	SocketTCP sender;
	sender.connect("127.0.0.1",11000);
	string senderString = "one two three.";
	sender.sendBlock(senderString.c_str(),senderString.length());

	SocketTCP reader;
	server.accept(&reader);

	assert_message("select failed",reader.select(0));
}

void SocketTCPtest::testAsyncSelect()throw(exception){

}

void SocketTCPtest::testGroupSelect()throw(exception){

	SocketTCP server;
	server.bind(11000);
	server.listen(10);

	for(int i=0; i<10;i++){
		SocketTCP sender;
		sender.connect("127.0.0.1",11000);
		string senderString = "one two three.";
		sender.sendBlock(senderString.c_str(),senderString.length());
	}

	std::vector<SocketTCP*> sockList;
	for(int i=0; i<10;i++){
		SocketTCP *reader=new SocketTCP();
		sockList.push_back(reader);
		server.accept(reader);
	}

	std::vector<SocketTCP*> selectedList = SocketTCP::groupSelect(sockList,0);
	assert_message("groupSelect did not select all sockets", selectedList.size()==sockList.size());

	std::vector<SocketTCP*>::iterator iter = sockList.begin();
	for(; iter!=sockList.end(); ++iter){
		delete *iter;
	}
}

void SocketTCPtest::RunTests() throw(exception){

	testServerAcceptClientConnect();
	testSelect_Zero();
	testAsyncSelect();
	testGroupSelect();
}

}

#ifndef SOCKET_H_
#define SOCKET_H_

#include <iostream>
#include <string>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

using namespace std;

class Socket {
public:
    Socket(const string& address, const string& port) :
	address(address), port(port), sock(0)
    {
	struct sockaddr_in serv_addr; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	    { 
		cerr << "Socket creation error" << endl;
		exit(1);
	    } 
   
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(stoi(port.c_str())); 
       
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, address.c_str(), &serv_addr.sin_addr)<=0)  
	    {
		cerr << "Invalid address/Address not supported" << endl;
		exit(1);
	    } 
   
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	    { 
		cerr << "Connection Failed" << endl;
		exit(1);
	    } 
    }

    void send(const string& message) {
	char *asd = "asd";
	send(sock , asd, message.length(), 0); 
    }

    string read() {
	char buffer[1024];
	read( sock , buffer, 1024);
	return string(buffer);
    }
private:
    const string& address, port;
    int sock;
};

#endif

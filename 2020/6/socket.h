#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdio.h> 
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <string.h>

class Socket {
public:
    Socket(const char *ip, size_t port) : errormsg(nullptr) {
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
	    strcpy(errormsg, "Socket creation error");
	} else {
	    serv_addr.sin_family = AF_INET; 
	    serv_addr.sin_port = htons(port);
	    
	    // Convert IPv4 and IPv6 addresses from text to binary form 
	    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <=0 ) {
		strcpy(errormsg, "Invalid address/Address not supported");
	    } else {
		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
		    strcpy(errormsg, "Connection failed");
		}
	    }
	}
    }

    const char* error() const { return errormsg; }

    void dismiss() {
	close(sock);
    }

    void send_msg(const char* msg) {
	send(sock, msg, strlen(msg) , 0 );
	
    }

    const char* read_msg() {
	read(sock , buffer, 1024);
	return buffer;
    }
    
private:
    char *errormsg;
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
};

#endif // SOCKET_H_

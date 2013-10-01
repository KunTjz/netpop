#ifndef __CONNECTION_H__
#define __CONNECTION_H__
#include <string>

#define TCP_CONNECTION_FILE 	"/proc/net/tcp"
#define UDP_CONNECTION_FILE 	"/proc/net/udp"
#define MAX_LINE		256

struct connection{
	int 		_inode;
	std::string 	_ip;
	unsigned int    _port;	
	struct connection* _next;
};

void getTcpCt (struct connection** head);
// void readUdpCt ();


#endif

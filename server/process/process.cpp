#include "process.h"
#include <netdb.h>
#include <iostream>
#include <cstdio>

void process::getPortByName ()
{
	struct servent* svr = getservbyname (_name.c_str (), "tcp");
	if (svr) {
		_port = ntohs (svr->s_port);
		printf ("0x%x\n", svr->s_port);
		std::cout << "port: " << _port << std::endl;
	}
	else {
		std::cout << "getservbyname failed\n";
		_port = 9999;
	}
}
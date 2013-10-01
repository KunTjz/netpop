#include "progress.h"
#include "prgNode.h"
#include "connection.h"
#include <netdb.h>
#include <iostream>
#include <cstdio>

extern struct node * prgNode[PRG_HASH_SIZE];

void process::getPortByName ()
{
	struct servent* svr = getservbyname (_name.c_str (), "tcp");
	if (svr) {
		_port = ntohs (svr->s_port);
		printf ("0x%x\n", svr->s_port);
		std::cout << "port: " << _port << std::endl;
	}
	else {
		std::cout << "getservbyname failed: " << _name << std::endl;
		buildCache ();
		struct connection* head = NULL;
		getTcpCt (&head);
		for (struct connection* p = head; p != NULL; p = p->_next) {
			for (struct node * q = prgNode[PRG_HASH_FUNC(p->_inode)]; q != NULL; 
				q = q->_next) {
				if (p->_inode == q->_inode) {
					if (q->_name == _name) {
						_port = p->_port; 
					}			
				}			
			}		
		}
	}
}

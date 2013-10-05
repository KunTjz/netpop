#include "progress.h"
#include "prgNode.h"
#include "connection.h"
#include <netdb.h>
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;
extern struct node* prgNode[PRG_HASH_SIZE];
struct connection* head = NULL;
vector<class process*> procs;

void buildProcessCache ()
{
	buildCache ();
	getTcpCt (&head);
	getUdpCt (&head);
	
	for (struct connection* p = head; p != NULL; p = p->_next) {
		for (struct node * q = prgNode[PRG_HASH_FUNC(p->_inode)]; 
			q !=  NULL;  q = q->_next) {
			if (p->_inode == q->_inode) {
				class process* proc = new process (q->_name
								  , p->_port
								  , q->_pid); 
				procs.push_back (proc);
			}			
		}		
	}
}

class process* getProcByName (const std::string& name)
{
	for (size_t i = 0; i < procs.size (); ++i) {
		if (procs[i]->getName () == name)
			return procs[i];
	}
	return NULL;
}

class process* getProcByPid (int pid)
{
	for (size_t i = 0; i < procs.size (); ++i) {
		if (procs[i]->getPid () == pid)
			return procs[i];
	}
	return NULL;
}

class process* getProcByPort (unsigned int port)
{
	for (size_t i = 0; i < procs.size (); ++i) {
		if (procs[i]->getPort () == port)
			return procs[i];
	}
	return NULL;
}

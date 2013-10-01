#ifndef __PRGNODE_H__
#define __PRGNODE_H__

#include <string>
#define PRG_HASH_SIZE		200
#define PRG_HASH_FUNC(x)	(x) % PRG_HASH_SIZE	

struct node{
	int 			_inode;		// ¶ÔÓŠµÄsocket±àºÅ
	unsigned int	_pid;		// œø³Ìpid
	std::string		_name;		// œø³ÌµÄÃû³Æ 
	struct node*	_next;		// ÏÂÒ»žönode
};

void addPrgNode (int inode, unsigned int pid, const std::string& name);
void clearCache ();
void buildCache ();

struct node* queryProNode (int inode);

#endif

#include "../util/sysDefine.h"
#include "prgNode.h"
#include <cstdlib>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cstring>
#include <vector>
#include <cstdio>
#include <iostream>
using namespace std;

#define PROC_PATH			"/proc"
#define MAX_LENGTH			256
#define PRG_SOCKET_PFX			"socket:["

struct node* prgNode[PRG_HASH_SIZE];

bool isPidDir (char* fullPath) {
	struct stat statBuf;
	if (lstat (fullPath, &statBuf) < 0) {
		NET_POP_EXCEPTION ("lstat %s failed, %s, %d", fullPath, __FILE__, __LINE__);
	}
	if (S_ISDIR (statBuf.st_mode) == 0) {
		return false;
	}

	char* name = strrchr (fullPath, '/');
	if (name == NULL) {
		return false;
	}
	++name;
	for (size_t i = 0; i < strlen(name); ++i) {
		if (! isdigit (name[i])) {
			return false;
		}
	}

	return true;
}

void pidDirHandle (char* path)
{
	int length = strlen (path);
	char* pr = path + length;
	*pr++ = '/';
	*pr = 0;

	strcpy (pr, "fd");
	struct stat statBuf;
	if (lstat (path, &statBuf) < 0) {
		NET_POP_EXCEPTION ("lstat %s failed, %s, %d", path, __FILE__, __LINE__);
	}
	if (S_ISDIR (statBuf.st_mode) == 0) {
		NET_POP_EXCEPTION ("%s is not a directory, %s, %d", path, __FILE__, __LINE__);
	}

	DIR* dp = opendir (path);
	if (dp == NULL) {
		NET_POP_EXCEPTION ("can not open %s, %s, %d", path, __FILE__, __LINE__);
	}

	struct dirent* dirp;
	vector<int> inodes;
	while ((dirp = readdir (dp)) != NULL) {
#ifdef DIRENT_HAVE_D_TYPE_WORKS
		if (dirp->d_type!= DT_LNK) {
			continue;
		}
#else
		if (strcmp (dirp->d_name, ".") == 0 || strcmp (dirp->d_name, "..") == 0) {
			continue;
		}
#endif
		char buf[MAX_LENGTH], fdPath[MAX_LENGTH];
		snprintf (fdPath, sizeof (fdPath), "%s/%s", path, dirp->d_name);
		int lnameLen = readlink (fdPath, buf, sizeof (buf));
		if (lnameLen < 0) {
			continue;
		}
		buf[lnameLen] = 0;
		
		int inode;
		if (strstr (buf, PRG_SOCKET_PFX) == NULL) {
			continue;
		}
		sscanf (buf, "socket:[%d]", &inode);
		inodes.push_back (inode);
	}
	
	if (inodes.size () == 0) {
		return;
	}
	pr = &path [length];
	snprintf (pr, sizeof (path) - sizeof (char) * (length + 1), "/cmdline");
	int fd = open (path, O_RDONLY);
	if (fd < 0) {
		return;
	}
	char buf[MAX_LENGTH], *name;
	int len = read (fd, buf, sizeof (buf));
	close (fd);
	if (len < 0) {
		return;
	}
	if (len < sizeof (buf) - 1) {
		buf [len] = 0;
	}
	if ((name = strrchr (buf, '/')) != NULL) {
		name++;
	}
	else {
		name = buf;
	}

	path[length] = 0;
	for (size_t i = 0; i < inodes.size (); ++i) {
		addPrgNode (inodes[i], atoi (strrchr (path, '/') + 1), name);
	}
}

void addPrgNode (int inode, unsigned int pid,const string& name)
{
	struct node* p = prgNode[PRG_HASH_FUNC(inode)];
	if (p != NULL) {	
		while (p->_next != NULL) {
			if (p->_inode == inode) {
				//NET_POP_EXCEPTION ("inode:%d is already been added, %s, %d", 					//	inode, __FILE__, __LINE
				// may get multiple processes for on inode
				return;		
			}
			p = p->_next;
		}
	}
	
	struct node* temp = new struct node;
	temp->_inode = inode;
	temp->_pid  = pid;
	temp->_name = name;
	temp->_next = NULL;

	if (prgNode[PRG_HASH_FUNC(inode)] == NULL) {
		prgNode[PRG_HASH_FUNC(inode)] = temp;	
	}
	else {
		p->_next = temp;
	}
}

struct node* queryProNode (int inode)
{
	struct node* p = prgNode[PRG_HASH_FUNC(inode)];
	for (; p != NULL; p = p->_next) {
		if (p->_inode == inode) {
			return p;
		}
	}
	return NULL;
}

void buildCache ()
{
	bzero (prgNode, sizeof (prgNode));

	//
	// 扫描/proc/pid来缓存所有进程信息
	//
	struct stat statBuf;
	if (lstat (PROC_PATH, &statBuf) < 0) {
		NET_POP_EXCEPTION ("lstat %s failed, %s, %d", PROC_PATH, __FILE__, __LINE__);
	}
	if (S_ISDIR (statBuf.st_mode) == 0) {
		NET_POP_EXCEPTION ("%s is not a directory, %s, %d", PROC_PATH, __FILE__, __LINE__);
	}
	DIR* dp = opendir (PROC_PATH);
	if (dp == NULL) {
		NET_POP_EXCEPTION ("can not open %s, %s, %d", PROC_PATH, __FILE__, __LINE__);
	}

	struct dirent* dirp;
	while ((dirp = readdir (dp)) != NULL) {
		if (strcmp (dirp->d_name, ".") == 0 || strcmp (dirp->d_name, "..") == 0) {
			continue;
		}

		char fullPath [MAX_LENGTH];
		snprintf (fullPath, sizeof (fullPath), "%s/%s", PROC_PATH, dirp->d_name);
		if (! isPidDir (fullPath)) {
			continue;
		}	
		
		pidDirHandle (fullPath);
	}

	if (closedir (dp) < 0) {
		NET_POP_EXCEPTION ("close %s failed, %s, %d", PROC_PATH, __FILE__, __LINE__);
	}

	// for test
	/*for (size_t i = 0; i < PRG_HASH_SIZE; ++i) {
		struct node* p = prgNode[i];
		if (p != NULL) {
		}
		if (i == 67) {
			while (p != NULL) {
				printf ("%d\n", p->_inode);
				p = p->_next;
			}
		}
	}*/
}

void clearCache ()
{
	for (size_t i = 0; i < PRG_HASH_SIZE; ++i) {
		struct node* p = prgNode[i];
		while (p != NULL) {
			struct node* temp = p;
			p = p->_next;
			delete temp;
		}
	}
}

#ifndef __UTIL_FUNC_H__
#define __UTIL_FUNC_H__

#include <cstdio>
#include <pcap.h>

/* handle the package */
void packageHandle(u_char *useless,
		  const struct pcap_pkthdr* pkthdr,
		  const u_char* packet);

void sigAlrm (int signo);
void sigInt (int signo);
void setAlarm (unsigned int time); 
int Atoi (char* str);

struct sysConfig {
	unsigned int _refreshDelay;
	char* _processName;
	int _processPid;
	int _speedLimit;
	
	sysConfig ():
		_refreshDelay (1),
		_processName (NULL),
		_processPid (-1),
		_speedLimit (-1) {}
};

#endif

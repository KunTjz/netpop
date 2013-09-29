#ifndef __CAPTURE_H__
#define __CAPTURE_H__
#include "filter/filter.h" 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h> 

typedef void (*callBack)(u_char *useless,const struct pcap_pkthdr* pkthdr,const u_char* packet);

class capturer {
public:
	capturer () {}
	capturer (std::string dev): _dev (dev) {}
	void capturePackage (filter* fr, callBack func);

private:
	std::string _dev; // Íø¿¨Éè±¸Ãû³Æ
};

#endif
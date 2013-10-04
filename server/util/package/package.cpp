#include "package.h"
#include "../../progress/progress.h"
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>

extern std::vector<class process*> procs;


class process* findProc (const u_char* packet, unsigned int DLT_Length, 
			 const std::string& ipAddr)
{
	// 14 is length of ethernet header , 16 is the length of ppp header
	ipHeader* ip = (ipHeader*)(packet + DLT_Length);
		
	char sourceIp[20], destinationIp[20];

	snprintf (sourceIp, 20, "%d.%d.%d.%d",  ip->sourceAddr.byte1,
					 	ip->sourceAddr.byte2, 
					 	ip->sourceAddr.byte3,
					 	ip->sourceAddr.byte4);

	snprintf (destinationIp, 20, "%d.%d.%d.%d", ip->destinationAddr.byte1,
					 	    ip->destinationAddr.byte2, 
					 	    ip->destinationAddr.byte3,
					 	    ip->destinationAddr.byte4);

	if (ip->proto == 0x06 ) {
		tcpHeader* tcp = (tcpHeader*)((u_char*)ip + (ip->ver_len & 0xf) * 4);
		if (sourceIp == ipAddr) {
			return getProcByPort (ntohs (tcp->sourcePort)); 		
		}
		else if (destinationIp == ipAddr) {
			return getProcByPort (ntohs (tcp->destinationPort)); 		
		}
		
	}
	else if (ip->proto == 0x17) {
		udpHeader* udp = (udpHeader*)((u_char*)ip + (ip->ver_len & 0xf) * 4);
		if (sourceIp == ipAddr) {
			return getProcByPort (ntohs (udp->sourcePort)); 		
		}
		else if (destinationIp == ipAddr) {
			return getProcByPort (ntohs (udp->destinationPort)); 		
		}	
	}

	return NULL;
}

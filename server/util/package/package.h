#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include <cstdlib>
#include <string>

typedef struct ipAddress {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ipAddress;

typedef struct ipHeader{
	u_char  ver_len; 
	u_char  typeOfService;
	u_short length;
	u_short identification;
	u_short flag_off;
	u_char  ttl;
	u_char  proto;
	u_short checkSum;
	ipAddress sourceAddr;
	ipAddress destinationAddr;
	u_int  	option;
} ipHeader; 

typedef struct tcpHeader {
	u_short sourcePort;
	u_short destinationPort;
	u_int	sequenceNo;
	u_int   ackNo;
	u_short off_reset_con;
	u_short windowSize;
	u_short checkSum;
	u_short urgentPointer;
	u_int 	option;
}tcpHeader;

typedef struct udpHeader {
	u_short sourcePort;
	u_short destinationPort;
	u_short length;
	u_short checkSum;
}udpHeader;

class process;
class process* findProc (const u_char* packet, unsigned int DLT_Length,
			 const std::string& ipaddr);

#endif

#include "sysDefine.h"
#include "utilFunc.h"
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <cctype>
#include <vector>
#include <cstring>
#include <ifaddrs.h>  
#include <arpa/inet.h>  
      
#include "../progress/progress.h"
#include "package/package.h"
#include "../dataHandler/dataHandler.h"

enum {
	ALARM_ON  = 1,
	ALARM_OFF = 2
};

static unsigned int alarmFlag = ALARM_OFF;
extern struct sysConfig config;
extern std::vector<class process*> procs;
int linkType;
std::string ip;

void setAlarm (unsigned int time)
{
	if (alarmFlag == ALARM_OFF) {
		alarm (time);
		alarmFlag = ALARM_ON;
	}
}

inline void cancleAlarm ()
{
	if (alarmFlag == ALARM_ON) {
		alarm (0);
		alarmFlag = ALARM_OFF;
		if (config._processName != NULL) {
			class process* proc = getProcByName (config._processName);
			if (proc != NULL)
				proc->bytesReset (0);
		}
		else if (config._processPid != -1) {
			class process* proc = getProcByPid(config._processPid);
			if (proc != NULL)
				proc->bytesReset (0);
		}
		else {
			for (size_t i = 0; i < procs.size (); ++i) {
				if (procs[i] != NULL)
					procs[i]->bytesReset (0);		
			}
		}
	}		
}

void killProcess (float speed, class process* proc)
{
	if (config._speedLimit < speed) {
		char cmd[50];
		snprintf (cmd, sizeof (cmd), "kill -9 %d", proc->getPid());
		system (cmd);
		printf ("the speed is over the limit:%d\n", config._speedLimit);
		printf (" process:%s has been killed", proc->getName().c_str());
		sigInt (SIGINT);
	}
}

void sigAlrm (int signo)
{
	system ("clear");
	printf ("pid\tport\tspeed\t\t\tname\n");
	if (config._processName != NULL
	   || config._processPid != -1) {
		class process* proc;
		if (config._processName != NULL)
			proc = getProcByName (config._processName);
		else 
			proc = getProcByPid (config._processPid);

		float speed = proc->getBytes () / ((float) 1024 * config._refreshDelay);
		printf ("%d\t%d\t%.3f KB/S\t\t%s\n" ,
			proc->getPid (), 
			proc->getPort (),
			speed,
			proc->getName ().c_str ());
		
		static dataHandler dh (proc->getName (), config._refreshDelay);
		dh.addData (speed);

		if (config._speedLimit != -1)
			killProcess (speed, proc);
	}
	else {
		for (size_t i = 0; i < procs.size (); ++i) {
			printf ("%d\t%d\t%.3f KB/S\t\t%s\n", 
				procs[i]->getPid (), 
				procs[i]->getPort (),
				procs[i]->getBytes () / ((float) 1024 * config._refreshDelay),
				procs[i]->getName ().c_str ());
		}
	}

	
	cancleAlarm ();
	setAlarm (config._refreshDelay);
}

void sigInt (int signo)
{
	alarm (0);
	delProcCache ();
	exit (0);
}

/* handle the package */
void packageHandle(u_char *u_handle,
				  const struct pcap_pkthdr* pkthdr,
				  const u_char*packet)
{
	unsigned int DLT_Length = 0;
	switch (linkType) {
		case (DLT_EN10MB):
			DLT_Length = 14;
			break;
		case (DLT_PPP):
			DLT_Length = 16;
			break;
		default:
			// TODO: maybe error? or other length?
			return;
	}

	if (config._processName == NULL && config._processPid == -1) {
		class process* proc = findProc (packet, DLT_Length, ip);
		if (proc == NULL) {
			return;		
		}
		proc->bytesAdd (pkthdr->len);
	}
	else {
		//class process* proc = findProc (packet, DLT_Length, ip);
		
		if (config._processName != NULL) {
			class process* proc = getProcByName (config._processName);
			proc->bytesAdd (pkthdr->len);
		}
		else if (config._processPid != -1) {
			class process* proc = getProcByPid(config._processPid);
			proc->bytesAdd (pkthdr->len);
		}
		else {
			NET_POP_EXCEPTION ("get process error, %s, %d", 
						__FILE__, __LINE__);		
		}
	}
}

int Atoi (char* str)
{
	char* p = str;
	while (*p != '\0') {
		if (!isdigit (*p++)) {
			NET_POP_EXCEPTION ("%s shoud be an integer", str);	
		}
	}

	return atoi (str);
}

int getNetInfo(char* devName, char* ipAddress)
{
        struct sockaddr_in *sin = NULL;
        struct ifaddrs *ifa = NULL, *ifList;

        if (getifaddrs(&ifList) < 0) return -1;

        for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)
        {
            if(ifa->ifa_addr->sa_family == AF_INET 
               && strcmp (ifa->ifa_name, "lo"))
            {
	        strcpy (devName, ifa->ifa_name);

                sin = (struct sockaddr_in *)ifa->ifa_addr;
                strcpy (ipAddress, inet_ntoa (sin->sin_addr));
		
		freeifaddrs (ifList);
		return 0;
            }
        }
        
        freeifaddrs(ifList);
        
        return -1;
}


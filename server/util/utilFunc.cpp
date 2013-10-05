#include "sysDefine.h"
#include "utilFunc.h"
#include <cstdlib>
#include <unistd.h>
#include <cctype>
#include <vector>
#include "../progress/progress.h"
#include "package/package.h"

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
			proc->bytesReset (0);
		}
		else if (config._processPid != -1) {
			class process* proc = getProcByPid(config._processPid);
			proc->bytesReset (0);
		}
		else {
			for (size_t i = 0; i < procs.size (); ++i) {
				procs[i]->bytesReset (0);		
			}
		}
	}		
}

void sigAlrm (int signo)
{
	system ("clear");
	printf ("pid\tport\tspeed\t\t\tname\n");
	if (config._processName != NULL) {
		class process* proc = getProcByName (config._processName);
		printf ("%d\t%d\t%.3f KB/S\t\t%s\n" ,
			proc->getPid (), 
			proc->getPort (),
			proc->getBytes () / ((float) 1024 * config._refreshDelay),
			proc->getName ().c_str ());
	}
	else if (config._processPid != -1) {
		class process* proc = getProcByPid(config._processPid);
		printf ("%d\t%d\t%.3f KB/S\t\t%s\n", 
			proc->getPid (), 
			proc->getPort (),
			proc->getBytes () / ((float) 1024 * config._refreshDelay),
			proc->getName ().c_str ());
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
			NET_POP_EXCEPTION ("%s shoud be a number", str);	
		}
	}

	return atoi (str);
}

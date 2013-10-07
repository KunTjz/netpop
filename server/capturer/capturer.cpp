#include "capturer.h"
#include <cstdlib>
#include <cstring>
#include <pcap.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

extern int linkType;
extern std::string ip;

static bool 
getCmdReturnStrings (const char* cmd, std::vector<std::string>& ret_lines) 
{ 
	if((NULL==cmd) || 0==strlen(cmd)) {
		return false;
	}
	
	ret_lines.clear (); 
	FILE * rp = popen (cmd, "r"); 
	if (NULL == rp) {
		return false;
	}

	char * line    = NULL; 
	size_t len	   = 0; 
	ssize_t result = 0; 
	while ((result = getline(&line, &len, rp)) != -1) { 
		if ((0 == line) || (0 == strlen (line))) {
			continue; 
		}

		std::string tmpLine(line);	
		ret_lines.push_back (tmpLine); 
	} 

	if (NULL != line) {
		free (line); 
	}
	pclose (rp); 

	return true; 
}

std::string getDevIp (const char* dev) 
{
	char cmd[80];
	snprintf (cmd, 80, "ifconfig %s| grep \"inet addr\"", dev);
	
	std::vector<std::string> lines;
	if (getCmdReturnStrings (cmd, lines) && lines.size () == 1) {
		sscanf (lines[0].c_str (), " inet addr:%s[^' ']", cmd);
		return cmd;	
	}
	return "";
}

capturer::capturer (const char* dev): _dev (dev) 
{
	ip = getDevIp (_dev.c_str ());
}

void capturer::capturePackage (filter* fr, callBack func, int delay)
{
	char errbuf [PCAP_ERRBUF_SIZE];
	pcap_t* descr;
	struct bpf_program fp;      /* hold compiled program     */

	try {
		if (! _dev.empty ()) {
			bpf_u_int32 maskp;          /* subnet mask               */
			bpf_u_int32 netp;           /* ip                        */

			/* ask pcap for the network address and mask of the device */
			pcap_lookupnet(_dev.c_str (), &netp, &maskp, errbuf);
			descr = pcap_open_live (_dev.c_str (), BUFSIZ, 1, -1, errbuf);
			linkType = pcap_datalink (descr);
		}
		else {
			descr = pcap_open_live (NULL, BUFSIZ, 1, -1, errbuf);
		}
		 /* open device for reading this time lets set it in promiscuous
		 * mode so we can monitor traffic to another machine             */
		
		if(descr == NULL) { 
			cout << "pcap_open_live():" << errbuf << endl; 
			exit(1); 
		}

		if (fr != NULL) {
			char* temp = new char [fr->getRule ().size () + 1];
			strcpy (temp, fr->getRule ().c_str ());
			if(pcap_compile (descr, &fp, temp, 0, 0) == -1) { 
				cout << "Error calling pcap_compile\n"; 
				exit(1); 
			}

			/* set the compiled program as the filter */
			if(pcap_setfilter(descr, &fp) == -1) { 
				cout << "Error setting filter\n"; 
				exit(1); 
			}
			delete temp;
		}

		setAlarm (delay);
		pcap_loop(descr,-1, func, NULL);
	}
	catch (exception& e) {
		cout << e.what () << endl;
		exit (0);
	}
}

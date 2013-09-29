#include "capturer.h"
#include <cstdlib>
#include <cstring>
#include <pcap.h>
#include <iostream>

void capturer::capturePackage (filter* fr, callBack func)
{
	using namespace std;

    char errbuf [PCAP_ERRBUF_SIZE];
    pcap_t* descr;
    const u_char *packet;
    struct pcap_pkthdr hdr;     /* pcap.h                    */
    struct ether_header *eptr;  /* net/ethernet.h            */
    struct bpf_program fp;      /* hold compiled program     */
 
	try {
		if (! _dev.empty ()) {
			bpf_u_int32 maskp;          /* subnet mask               */
			bpf_u_int32 netp;           /* ip                        */

			/* ask pcap for the network address and mask of the device */
			pcap_lookupnet(_dev.c_str (), &netp, &maskp, errbuf);
			descr = pcap_open_live (_dev.c_str (), BUFSIZ, 1, -1, errbuf);
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

		char* temp = new char (fr->getRule ().size () + 1);
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

		pcap_loop(descr,-1, func, NULL);
	}
	catch (exception& e) {
		cout << e.what ();
	}

}
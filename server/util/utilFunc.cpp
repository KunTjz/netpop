#include "utilFunc.h"
#include <cstdlib>

/* handle the package */
void packageHandle(u_char *useless,
				  const struct pcap_pkthdr* pkthdr,
				  const u_char*packet)
{
	static int count = 1;
	fprintf(stdout,"%d, length: %d ",count, pkthdr->len);
	fflush(stdout);
	count++;
}

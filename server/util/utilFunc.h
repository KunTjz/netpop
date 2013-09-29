#ifndef __UTIL_FUNC_H__
#define __UTIL_FUNC_H__

#include <cstdio>
#include <pcap.h>

/* handle the package */
void packageHandle(u_char *useless,
				  const struct pcap_pkthdr* pkthdr,
				  const u_char*packet);
#endif
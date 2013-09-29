#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "../process/process.h"
#include "../capturer/filter/filter.h"
#include "../capturer/capturer.h"
#include "../util/utilFunc.h"

int main (int argc, char* argv[])
{
	std::string processName = argv[1];
	process proc (processName);

	char temp[30];
	sprintf (temp, "port %d", proc.getPort ());
	filter fr;
	fr.setRule (temp);

	capturer ct;
	ct.capturePackage (&fr, packageHandle);

	return 0;
}
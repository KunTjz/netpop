#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "../progress/progress.h"
#include "../capturer/filter/filter.h"
#include "../capturer/capturer.h"
#include "../util/utilFunc.h"
#include "../util/sysDefine.h"

int main (int argc, char* argv[])
{
	try {
		std::string processName = argv[1];
		process proc (processName);

		char temp[30];
		sprintf (temp, "port %d", proc.getPort ());
		filter fr;
		fr.setRule (temp);

		capturer ct;
		ct.capturePackage (&fr, packageHandle);
	}
	catch (myException& e) {
		std::cout << e.toString () << "\n";
	}


	return 0;
}

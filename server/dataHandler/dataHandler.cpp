#include "dataHandler.h"
#include "../util/sysDefine.h"
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

dataHandler::dataHandler (std::string name, int delay): 
	_procName (name), _refreshDelay (delay)
{
	_counts = 0;
	
	_of.open ("./data/data.txt", std::ios::out | std::ios::trunc);
	_of << _procName << " " << _refreshDelay << std::endl;
}


void dataHandler::addData (float speed)
{
	
	_of << speed;
	if (++_counts == 20) {
		_of << std::endl;
		_counts = 0;
		return;
	}
	_of << ' ';
}


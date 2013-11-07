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
	reset ();
	_fd = open ("./data/data.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	if (_fd < 0) {
		NET_POP_EXCEPTION ("can not open %s, %s, %d", "./data/data.txt", __FILE__, __LINE__);
	}

	if (write (_fd, _procName.c_str (), _procName.length ()) < 0
	    || write (_fd, &_refreshDelay, sizeof (int)) < 0) 
		NET_POP_EXCEPTION ("can not write %s, %s, %d", "./data/data.txt", __FILE__, __LINE__);
}

void dataHandler::reset ()
{
	 _counts = 0;
         for (size_t i = 0; i < MAX_DATA_NUM; i++) {
         	_data[i] = -1;          
         }
}

void dataHandler::addData (float speed)
{
	if (_counts < MAX_DATA_NUM) {
		_data[_counts++] = speed;  
	}
	else {
		recordData ();
		reset ();
	}
}

void dataHandler::recordData ()
{
	if (write (_fd, _data, sizeof (float) * _counts) < 0) 
		NET_POP_EXCEPTION ("can not write %s, %s, %d", "./data/dat.txt", __FILE__, __LINE__);
}

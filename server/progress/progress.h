#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <string>

enum {
	GET_PORT_BY_NAME = 1,
	GET_PORT_BY_PID,
};

class process{
private:
	std::string		_name; // ������
	unsigned int		_port; // ����ʹ�õĶ˿ں�
	int 			_pid;  // process pid numbe
	unsigned int 		_bytes;  

private:
	//void getServiceInfo ();
	//void getPort (struct connection* head, unsigned int pattern);

public:
	process (const std::string& name, unsigned int port, int pid): 
		_name (name),
		_port (port),
		_pid (pid),
		_bytes (0) {
			
	}

	unsigned int getPort () {
		return _port;
	}
	std::string getName () {
		return _name;
	}
	int getPid () {
		return _pid;
	}
	unsigned int getBytes () {
		return _bytes;	
	}
	void bytesAdd (unsigned int count) {
		_bytes += count;
	}
	void bytesReset (int number) {
		_bytes = number;	
	}	
};

void buildProcessCache ();
class process* getProcByName (const std::string& name);
class process* getProcByPid (int pid);
class process* getProcByPort (unsigned int port);

#endif

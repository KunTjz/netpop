#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <string>

enum {
	GET_PORT_BY_NAME = 1,
	GET_PORT_BY_PID,
};

class process{
private:
	std::string		_name; // processName
	unsigned int		_port; // localPort
	std::string		_remIp;	// connected process's remote ip 
	unsigned int 		_remPort; // connected process's remote port
	int 			_pid;  // process pid numbe
	unsigned int 		_bytes;  

private:
	//void getServiceInfo ();
	//void getPort (struct connection* head, unsigned int pattern);

public:
	process (const std::string& name, unsigned int port, const std::string& ip, unsigned int remPort, int pid): 
		_name (name),
		_port (port),
		_remIp (ip),
		_remPort (remPort),
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
	std::string getRemIp () {
		return _remIp;
	}
	unsigned int getRemPort () {
		return _remPort;
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
void delProcCache ();

class process* getProcByName (const std::string& name);
class process* getProcByPid (int pid);
class process* getProcByPort (unsigned int port);
class process* getProcByConInfo (const std::string& sourceIp, 
				 const std::string& dstIp,
				 unsigned int sourcePort,
				 unsigned int dstPort);
#endif

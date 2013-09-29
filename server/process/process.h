#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <string>

class process{
private:
	std::string		_name; // 进程名
	unsigned int	_port; // 进程使用的端口号

private:
	void getPortByName ();

public:
	process (std::string name): _name (name) {
		getPortByName ();
	}
	process (char* name, unsigned int port): _name (name), _port (port) {}

	unsigned int getPort () {
		return _port;
	}
	std::string getName () {
		return _name;
	}
};
#endif
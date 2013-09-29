#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <string>

class process{
private:
	std::string		_name; // ������
	unsigned int	_port; // ����ʹ�õĶ˿ں�

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
#ifndef _DATA_HANDLER_
#define _DATA_HANDLER_

#include <string>

class dataHandler {
public:
	explicit dataHandler (std::string name, int delay);
		
	~dataHandler () {
		if (_fd)
			close (_fd);
	}

	void addData (float speed);
private:
	// do not copy me
	dataHandler (const dataHandler& dh);
	dataHandler& operator= (const dataHandler& dh);		
	
	void recordData ();
	void reset ();
private:
	enum {
		MAX_DATA_NUM = 2
	};

	std::string _procName;
	int _refreshDelay;
	float _data[MAX_DATA_NUM];
	int   _counts;
	int   _fd;
};

#endif

#ifndef _DATA_HANDLER_
#define _DATA_HANDLER_

#include <fstream>
#include <string>

class dataHandler {
public:
	explicit dataHandler (std::string name, int delay);
		
	~dataHandler () {
		if (_of)
			_of.close ();
	}

	void addData (float speed);
private:
	// do not copy me
	dataHandler (const dataHandler& dh);
	dataHandler& operator= (const dataHandler& dh);		
	
private:

	std::string _procName;
	int _refreshDelay;
	std::ofstream _of;
	int _counts;
};

#endif

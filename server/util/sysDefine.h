#ifndef __SYS_DEFINE_H__
#define __SYS_DEFINE_H__

#include <cstdio>
#include <string>

#define ERROR_SIZE 100

class myException
{
public:
	myException (char* errmsg): _errmsg (errmsg) {}
	myException (std::string errmsg): _errmsg (errmsg) {}
	std::string& toString () {
		return _errmsg;	
	} 
private:
	std::string _errmsg;

};

#define NET_POP_EXCEPTION(str, args...)	do {	\
													char errMsg[ERROR_SIZE];	\
													snprintf (errMsg, sizeof (errMsg), str, ##args);	\
													throw myException(errMsg);	\
											} while (false);

#endif

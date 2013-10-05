#ifndef __SYS_DEFINE_H__
#define __SYS_DEFINE_H__

#include <cstdio>
#include <string>
#include <stdexcept>
#define ERROR_SIZE 100

class myException:public std::exception
{
public:
	explicit myException (char* errmsg) throw ():  _errmsg (errmsg) {}
	explicit myException (std::string errmsg) throw (): _errmsg (errmsg) {}
	~ myException () throw () {} // must add throw ()
	const char* what () {
		return _errmsg.c_str ();	
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

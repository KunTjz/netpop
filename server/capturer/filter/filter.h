#ifndef __FILTER_H__
#define __FILTER_H__
#include <string>

class filter {
public:
	void setRule (std::string rule) {
		_rule = rule;
	}
	std::string getRule () {
		return _rule;
	}

private:
	std::string _rule; // ¹ıÂËÔ­Ôò
};

#endif
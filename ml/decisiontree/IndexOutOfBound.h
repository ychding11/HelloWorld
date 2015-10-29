
#ifndef _INDEX_OUT_OF_BOUND_H_
#define _INDEX_OUT_OF_BOUND_H_

#include <stdexcept>

class IndexOutOfBound: public std::exception 
{
private:
	int __mindex;
public:
	IndexOutOfBound(int index);
	const char* what() const throw();
	~IndexOutOfBound() throw();
};

#endif

#include "Logger.h"

int main(void)
{
	Logger log("./log.txt");
	log << "This is a test!" << std::endl;
	return 0;
}

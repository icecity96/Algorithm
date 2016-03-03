#include "base_list.h"
#include <iostream>
int main(int argc, char *argv[])
{
	IceCity::sklist<int, int> ss;
	
	for (int i = 0; i < 20; ++i) {
		ss.insert(i, i*10);	
	}

	ss.print();
	ss.delete_key(5);
	try{
		ss[5];
	}catch(...)
	{
		std::cout << "error!" << std::endl;
	}
	return 0;
}

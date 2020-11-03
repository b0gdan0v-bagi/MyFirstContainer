#include <iostream>
#include "MyContainer.h"
#include <vector>

int main()
{
	MyFirstContainerClass<int> v;
	std::vector<int> vec;
	vec.begin();
	v.push_back(1);
	v.push_back(2);
	v.print();
}


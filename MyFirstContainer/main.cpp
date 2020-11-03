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
	MyFirstContainerClass<int> v2;
	v2.setWall(-2);

	
	for (int i = 0; i < 10; i++) v2.push_back(i+10);
	std::cout << v2.size() << " " << v2.getWallIndex() << "\n";
	for (int i = 0; i < 20; i++)  std::cout << " " << v2.at(i-5);
	std::cout << std::endl;
	v2.setLoop();
	for (int i = 0; i < 20; i++) std::cout << i-5 << " " << v2.at(i - 5) << "\n";
	std::cout << v[1];
}


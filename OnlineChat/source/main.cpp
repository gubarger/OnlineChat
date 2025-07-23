#include "include/BaseInit.h"
#include "include/Interface.h"

#include <exception>
#include <iostream>

int main() {
	BaseInit init;
	Interface _interface;

	int wDisplay = 1280, hDisplay = 720;

	try
	{
		init.initialize("Online Chat", wDisplay, hDisplay, &_interface);
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << "\n";
		return -1;
	}

	return 0;
}
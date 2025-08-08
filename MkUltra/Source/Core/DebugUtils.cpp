#include "DebugUtils.h"
#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif

void mk::Print(const std::string& text)
{
#ifdef WIN32
	OutputDebugString(text.c_str());
#endif
	std::cout << text << std::endl;
}

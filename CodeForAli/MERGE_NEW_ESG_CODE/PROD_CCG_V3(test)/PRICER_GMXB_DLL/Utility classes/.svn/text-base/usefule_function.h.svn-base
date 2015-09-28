#pragma once 

#pragma once
#include <iostream>
#include <sstream>
#include <string>

class Converter
{
public:
	template<typename T>
	static std::string convert_to_str(T& ip)
	{
		std::stringstream ss;
		ss << ip;
	    
		std::string s;
		s = ss.str();
		return s;
	}
};
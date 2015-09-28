#pragma once
#ifndef DEBUG_H
#define DEBUG_H
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
using namespace std;
/**
* Class that contain a debug file 
*/
class Debug
{
public:
	ofstream debug_file;
	Debug(void);
	~Debug(void);
	
};
#endif

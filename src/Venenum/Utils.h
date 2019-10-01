#pragma once
#include <fstream>

class Utils
{
public:
	static bool fileExists(const char *filename)
	{
	  std::ifstream ifile(filename);
	  return ifile.is_open();
	}
};

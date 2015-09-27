#ifndef __GMXB_LOGGER_H_
#define __GMXB_LOGGER_H_

#include "stdafx.h"
//#include "Utility classes/GMXBException.h"

class GMXBLogger
{
	private:
		ofstream log_file;
		char * file_name;

	public:
		~GMXBLogger()
		{
			this->log_file.close();
		}
		GMXBLogger(const char* name)
		{
			log_file.open(name);
			if (!log_file)
			{
				 char error_msg[100];
				 sprintf_s(error_msg, "The file %s is not found", name);
				 throw new GMXBException(error_msg);
			}

		}
		void debug(const string& trace)
		{
			this->log_file << trace << endl;
		}
};

#endif
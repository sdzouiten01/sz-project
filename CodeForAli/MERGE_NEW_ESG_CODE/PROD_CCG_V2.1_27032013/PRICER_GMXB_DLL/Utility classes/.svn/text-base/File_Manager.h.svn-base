#pragma once

#include "../stdafx.h"
#include "./head.h"
#include "GMXBException.h"
#include <io.h>   
#include <sys/types.h>  
#include <sys/stat.h>   

using namespace std;

class File_Manager
{
	public :
		~File_Manager()
		{
		}
		File_Manager()
		{		
		}

		void is_file_path_valid(const string& file_name_path)
		{
			ifstream file_op(file_name_path.c_str() , ios::in);
			if (!file_op)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "The file %s is not found", file_name_path.c_str());
				throw new GMXBException(message);
			}
			file_op.close();
		}


		void is_directory_path_valid(const string& directory_path)
		{
		    if ( access( directory_path.c_str(), 0 ) != 0 )
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "The directory %s doesn't exist", directory_path.c_str());
				throw new GMXBException(message);
			}
		}

		void is_directory_path_valid_2(const string& directory_path, const string& param)
		{
		    if ( access( directory_path.c_str(), 0 ) != 0 )
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "The directory %s doesn't exist parama %s", directory_path.c_str(), param.c_str());
				throw new GMXBException(message);
			}
		}
};
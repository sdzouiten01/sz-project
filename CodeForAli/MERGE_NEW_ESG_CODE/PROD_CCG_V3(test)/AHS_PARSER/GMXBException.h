#pragma once
#define MAX_LENGTH_MESSAGE_EXCEPTION 1000
#define  UNREFERENCED_PARAMETER(P) (P)
class GMXBException
{
	private:
		char * message;

	public:
		~GMXBException();

		GMXBException(char* msg);

		char * get_message();
};

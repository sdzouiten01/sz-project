#pragma once

#include <string>
#include <vector>
#include <map>
#include "stdafx.h"
#include "GMXBExcelXMLReader.h"
using namespace std;

class WildcardManager
{
private :

	map<string, string> mWildCardList;
	/*const*/ string leftParenthesis;
	/*const*/ string rightParenthesis;
	/*const*/ int lenParenthesis;

public:

	WildcardManager( const string& fileName, const string& sheetName ):
		leftParenthesis( "<*" ), rightParenthesis( "*>" ), lenParenthesis( 2 )
	{
		GMXBExcelXMLReader xmlFile( fileName );
		map<string, vector<string> > _mWildCardList;
		map<string, vector<string> >::iterator itr;

		xmlFile.parse_XML_sheet( sheetName, _mWildCardList );

		// convert map<string, vectot<string> > to map<string, string>
		for(itr=_mWildCardList.begin(); itr!=_mWildCardList.end(); ++itr)
			this->mWildCardList[itr->first] = itr->second[0];
	}

	~WildcardManager()
	{
	}

	// string& getPath( string& path )  Ali
	string getPath( string path )   
	{
		size_t left = path.find( this->leftParenthesis );
		size_t right = path.find( this->rightParenthesis ) + this->lenParenthesis;

		if (left == string::npos || right == string::npos)
			return path;
		else
			return getPath( path.replace(left, right-left, this->mWildCardList[path.substr(left+this->lenParenthesis,right-left-2*this->lenParenthesis)]));
	}
};

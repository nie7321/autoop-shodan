#include "CIniFile.h"
#include <windows.h>
#include <stdlib.h>

CIniFile::CIniFile()
{
	fileName = "";	//Initialize filename to nothing.
}

CIniFile::~CIniFile()
{
}

void CIniFile::SetFileName(char *file)
{
	fileName = file;
}

std::string CIniFile::GetFileName()
{
	return fileName;
}

unsigned int CIniFile::GetString(char *section,char *keyname,char *str, int buflen)
{
	return GetPrivateProfileString(section,keyname,"NULL",str,buflen,fileName.begin());
}

unsigned int CIniFile::GetStringD(char *section,char *keyname,char *str, int buflen, char *mydefault)
{
	return GetPrivateProfileString(section,keyname,mydefault,str,buflen,fileName.begin());
}

unsigned int CIniFile::GetIntD(char *section,char *keyname, int imydefault)
{
	return GetPrivateProfileInt(section,keyname,imydefault,fileName.begin());
}

float CIniFile::GetFloatD(char *section,char *keyname, char *mydefault)
{
	char szBuffer[25];
	
	GetPrivateProfileString(section,keyname,mydefault,szBuffer,20,fileName.begin());

	return atof(szBuffer);
}

unsigned int CIniFile::GetInt(char *section,char *keyname)
{
	return GetPrivateProfileInt(section,keyname,999,fileName.begin());
}

bool CIniFile::SetString(char *section,char *keyname,char *string)
{
	if(WritePrivateProfileString(section,keyname,string,fileName.begin()))
		return true;
	else
		return false;
}

bool CIniFile::SetInt(char *section, char *keyname, unsigned int val)
{
	char *string = new char[255];
    itoa(val,string,10);
    if(WritePrivateProfileString(section,keyname,string,fileName.begin()))
   	{
		delete [] string;
		return true;
	}
	else
	{
		delete [] string;
		return false;
	}
}

#include <string>
class CIniFile


    {
    public:
    	bool SetInt(char *section,char *keyname,unsigned int val); //Change the value of a integer in the file.
    	bool SetString(char *section,char *keyname,char *string); //Change the value of a string in the file.
    	unsigned int GetInt(char *section,char *keyname); //Get the value of an unsigned integer in the file.
    	unsigned int GetString(char *section,char *keyname,char *str,int buflen); //Get the value of an string in the file.
		unsigned int GetStringD(char *section,char *keyname,char *str, int buflen, char *mydefault);
		unsigned int GetIntD(char *section,char *keyname, int imydefault);
		float GetFloatD(char *section,char *keyname, char *mydefault);
    	std::string GetFileName();	//Get the current file name.
    	void SetFileName(char *);	//Set the current file name.
    	CIniFile();		//Default constructor
    	~CIniFile();	//Default destructor
    private:
    	std::string fileName; //Internal file name.
    	
};
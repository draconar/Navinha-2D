#ifndef _ERRORLOGMANAGER_
#define _ERRORLOGMANAGER_

// Error Manager is composed of 2 classes:
// the singleton CErrorLogManager + the exception throwing CException

//#include "EngineObject.h"
#include <string>
#include <sstream>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <exception>

/**
	Throw macro: THROWs a CException class
	use file & line constants
****/

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(ErrorNum, ErrorDescrp) throw	CException(ErrorNum, ErrorDescrp, __FILE__, __LINE__);
#endif

/***
	CException: does it works for every exception thrown?
	- extends std::exception
***/

class CException: public std::exception {
public:
	int errorNumber;
	std::string errorDesc;
	std::string srcFileName;
	int lineNumber;
	std::string errText;

	//overrided std::exception 'what()'
	const char* what();

	//ctor
	CException(int errorNumber, std::string errorDesc, std::string srcFileName, int lineNumber);
};

/**
	CErrorManager: Singleton class that manages the errors throw by the app	
***/

class CErrorLogManager /*: public EngineObject */ 
{
public:
	static CErrorLogManager* getErrorManager();
	std::stringstream logBuffer;
	void create(std::string fileName);
	void flush();
	void close();
	//logs an exception to the file
	void logException(CException e);
	//get time as string
	std::string getTimeString();
	std::ofstream logFile;

private:
	//private ctor, as required by the Singleton DP
	CErrorLogManager(){};
	static CErrorLogManager errorManager;

protected:
	virtual ~CErrorLogManager(){};

};

#endif
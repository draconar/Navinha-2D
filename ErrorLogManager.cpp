//impl. of exception and the singleton ErrorlogManager
#include "ErrorLogManager.h"

#include "debug.h"

/**
	Impl. of CException
****/

CException::CException(int eN, std::string eD, std::string srcFN, int ln): errorNumber(eN), errorDesc(eD), srcFileName(srcFN), lineNumber(ln) {
	std::stringstream errStr;

	errStr << "ERror num: " << errorNumber << "\nError Desc: " << errorDesc << "\nSrc File: " << srcFileName << "\nLine #: " << lineNumber << '\n'; 

	errText = errStr.str();
}

const char* CException::what() {
	return errText.c_str();
}

/**
	Impl. of CErrorLogManager
****/

//create instance of the singleton
CErrorLogManager CErrorLogManager::errorManager;

void CErrorLogManager::create(std::string fileName) {
	logFile.open(fileName.c_str());
}

//commit info to file & clears text cache
void CErrorLogManager::flush() {
	logFile << logBuffer.str();
	logFile.flush();
	logBuffer.str("");
}

void CErrorLogManager::close() {
	logFile.close();
}

//time in hh:mm:ss
std::string CErrorLogManager::getTimeString() {
	std::stringstream timeStr;

	struct tm *pTime;
	time_t ctTime; time(&ctTime);
	pTime = localtime(&ctTime);
	
	timeStr << std::setw(2) << std::setfill('0') << pTime->tm_hour  << ':';
	timeStr << std::setw(2) << std::setfill('0') << pTime->tm_min  << ':';
	timeStr << std::setw(2) << std::setfill('0') << pTime->tm_sec;

	return timeStr.str();

}


//writes exception to log
void CErrorLogManager::logException(CException e){
	logBuffer << getTimeString() << '\n' << e.what();
	logFile.flush();
}

CErrorLogManager* CErrorLogManager::getErrorManager() {
	return &errorManager;
}
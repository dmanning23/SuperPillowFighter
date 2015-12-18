/*////////////////////////////////////////////////////////////////////////
//File: Logger.cpp
//
//Author: Dan Manning
//
//Creation Date: March 1, 2004
//
//Purpose: Log various information to a file
//
/////////////////////////////////////////////////////////////////////////*/

#include "logger.h"

/*////////////////////////////////////////////////////////////////////////
//Last Modified: March 2, 2004
//Purpose: Create a Logger, set up file for writing
//
/////////////////////////////////////////////////////////////////////////*/
CLogger::CLogger() : m_strFileName(0)
{
}

/*////////////////////////////////////////////////////////////////////////
//Last Modified: March 1, 2004
//Purpose:	Destruct a Logger.  Should not be used to delete the pointer
//			pm_Instance, since this would crash the whole program
/////////////////////////////////////////////////////////////////////////*/
CLogger::~CLogger(void)
{
#ifdef _DEBUG
	//make sure file is closed
	if (fout.is_open())
	{
		fout.flush();
		fout.close();
	}
#endif
}

/*////////////////////////////////////////////////////////////////////////
//Function: LoggerInit
//Purpose: set up the file for writing
//
/////////////////////////////////////////////////////////////////////////*/
bool CLogger::Init(char *strFileName)
{
#ifdef _DEBUG

	m_strFileName = (char*)malloc(strlen("LogFiles\\") + strlen(strFileName) + 1);
	//set m_strFilename to the path to logfiles
	strcpy(m_strFileName, "LogFiles\\");
	strcat(m_strFileName, strFileName);

	//open file
	fout.open(m_strFileName, ios_base::out | ios_base::app);

	//double check that it worked
	if (!fout.is_open())
	{
		cerr << "Can't open " << m_strFileName << " for writing.";
		return false;
	}
	else
	{
		//add date and time here
		timestamp();
		return true;
	}
#endif
	return true;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	LoggerShutdown
//Purpose:	close file
//
/////////////////////////////////////////////////////////////////////////*/
void CLogger::Shutdown()
{
#ifdef _DEBUG
	//close file
	fout.flush();
	fout.close();

	free(m_strFileName);
#endif
}

/*////////////////////////////////////////////////////////////////////////
//Function:	timestamp
//Purpose:	Put the date and time in the log file
//
/////////////////////////////////////////////////////////////////////////*/
void CLogger::timestamp()
{
#ifdef _DEBUG
	time_t currentTime;
	time(&currentTime);
	*this << "\n\n" << ctime(&currentTime);
#endif
}
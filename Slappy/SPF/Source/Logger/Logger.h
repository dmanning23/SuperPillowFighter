#ifndef _LOGGER_H_
#define _LOGGER_H_

/*////////////////////////////////////////////////////////////////////////
//File: Logger.h
//
//Author: Dan Manning
//
//Creation Date: March 1, 2004
//
//Purpose: Log various information to a file
//
/////////////////////////////////////////////////////////////////////////*/

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;
#endif

class CLogger
{
	private:


		//Character pointer to hold filename
		char *m_strFileName;

#ifdef _DEBUG
		//ofstream used to text out to file
		ofstream fout;
#endif
		
		/*////////////////////////////////////////////////////////////////////////
		//Function: Copy Constructor
		//
		//Last Modified: March 1, 2004
		//
		//Input: 
		//LoggerRef: const reference to the Logger to copy
		//
		//Output: NONE
		//
		//Return: NONE
		//
		//Purpose:	Copy a Logger into another Logger.
		//			It's a singleton, this function should never be used
		/////////////////////////////////////////////////////////////////////////*/
		CLogger(const CLogger &LoggerRef) {}

		/*////////////////////////////////////////////////////////////////////////
		//Function: Assignment Operator
		//
		//Last Modified: March 1, 2004
		//
		//Input:
		//LoggerRef: const reference to the Logger to copy
		//
		//Output: NONE
		//
		//Return: NONE
		//
		//Purpose:	Copy a Logger into another class.
		//			Since this is a singleton, this function should never be used.
		/////////////////////////////////////////////////////////////////////////*/
		CLogger &operator=(const CLogger &LoggerRef) {}

		/*////////////////////////////////////////////////////////////////////////
		//Function:	timestamp
		//
		//Last Modified:	march 3, 2004
		//
		//Input:	NONE
		//
		//Output:	NONE
		//
		//Return:	NONE
		//
		//Purpose:	Put the date and time in the log file
		//
		/////////////////////////////////////////////////////////////////////////*/
		void timestamp();

	public:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Default constructor
		//
		//Last Modified:	march 6, 2004
		//
		//Input:	NONE
		//
		//Output:	NONE
		//
		//Return:	NONE
		//
		//Purpose:	Constructs logger class, initializes file name
		//
		/////////////////////////////////////////////////////////////////////////*/
		CLogger();

		/*////////////////////////////////////////////////////////////////////////
		//Function: Destructor
		//
		//Last Modified: March 1, 2004
		//
		//Input: NONE
		//
		//Output: NONE
		//
		//Return: NONE!
		//
		//Purpose:	Destruct a Logger.  Should not be used to delete the pointer
		//			pm_Instance, since this would crash the whole program
		/////////////////////////////////////////////////////////////////////////*/
		virtual ~CLogger(void);

		/*////////////////////////////////////////////////////////////////////////
		//Function:  Overloaded << operator for text-out to a file
		//
		//Last Modified: march 2, 2004
		//
		//Input:
		// outToFile:  anything that can be outputted using ofstream
		//
		//Output: NONE
		//
		//Return: 
		//	Logger&:  this function can be shotgunned
		//
		//Purpose:	intuitive use of << operator to write to a log file
		//
		/////////////////////////////////////////////////////////////////////////*/
		template <typename any>
		CLogger &operator<<(any outToFile);

		/*////////////////////////////////////////////////////////////////////////
		//Function: LoggerInit
		//
		//Input:
		//	strFileName:	file to write to
		//
		//Output: NONE
		//
		//Return: 
		//	true if successful
		//	false if unsuccessful
		//
		//Purpose: set up the file for writing
		//
		/////////////////////////////////////////////////////////////////////////*/
		bool Init(char *strFileName);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	LoggerShutdown
		//
		//Last Modified: march 2, 2004
		//
		//Input: NONE
		//
		//Output: NONE
		//
		//Return: NONE
		//
		//Purpose:	close file
		//
		/////////////////////////////////////////////////////////////////////////*/
		void Shutdown();
};

/*////////////////////////////////////////////////////////////////////////
//Function:  Overloaded << operator for text-out to a file
//Purpose:	intuitive use of << operator to write to a log file
//
/////////////////////////////////////////////////////////////////////////*/
template <typename any>
CLogger &CLogger::operator<<(any outToFile)
{
#ifdef _DEBUG
	fout << outToFile;
	fout.flush();
#endif
	return *this;
}

#endif
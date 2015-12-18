/*////////////////////////////////////////////////////////////////////////
//File:	StateManager.cpp
//
//Author:	Dan Manning
//
//Creation Date:	March 12, 2004
//
//Purpose:	Function definitions of state manager class
//
/////////////////////////////////////////////////////////////////////////*/

#include "State_Manager.h"
#include "..\\MemManager\\MemManager.h"

/*////////////////////////////////////////////////////////////////////////
//Function:	Dummy Copy Constructor
//
//Last Modified:	March 13, 2004
//
//Purpose:	Copy a CStateMachine into a new one
//
/////////////////////////////////////////////////////////////////////////*/
CStateMachine::CStateMachine(const CStateMachine &CStateMachineRef)
{
	//get dimensions of state table
	m_Messages = CStateMachineRef.m_Messages;
	m_States = CStateMachineRef.m_States;

	//set up dynamic memory
	m_data = (int**)MemMalloc(m_States * sizeof(int));

	for (int i = 0; i < m_States; i++)
	{
		m_data[i] = (int*)MemMalloc(m_Messages * sizeof(int));
	}

	//set state table to no change in states
	for (int i = 0; i < m_States; i++)
	{
		for (int j = 0; j < m_Messages; j++)
		{
			m_data[i][j] = CStateMachineRef.m_data[i][j];
		}
	}

	this->m_currentState = CStateMachineRef.GetCurrentState();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Dummy Assignment Operator
//
//Last Modified:	March 12, 2004
//
//Purpose:	Should never be called
//
/////////////////////////////////////////////////////////////////////////*/
CStateMachine &CStateMachine::operator=(const CStateMachine &CStateMachineRef)
{
	//LOGFILE << "Illegal function call: CStateMachine Assignment Operator";
	return *this;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Constructor
//
//Last Modified:	March 12, 2004
//
//Purpose:	Create a CState_Machine
//
/////////////////////////////////////////////////////////////////////////*/
CStateMachine::CStateMachine(int numStates, int numMessages, int initialState) 
	: m_States(numStates), m_Messages(numMessages), m_currentState(initialState)
{
	//set up dynamic memory
	m_data = (int**)MemMalloc(m_States * sizeof(numStates));

	for (int i = 0; i < numStates; i++)
	{
		m_data[i] = (int*)MemMalloc(m_Messages * sizeof(int));
	}

	//set state table to no change in states
	for (int i = 0; i < numStates; i++)
	{
		for (int j = 0; j < numMessages; j++)
		{
			m_data[i][j] = i;
		}
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	destructor
//
//Last Modified:	March 12, 2004
//
//Purpose:	Destroy a CStateMachine
//
/////////////////////////////////////////////////////////////////////////*/
CStateMachine::~CStateMachine(void)
{
	for (int i = 0; i < m_States; i++)
	{
		MemFree(m_data[i]);
	}

	MemFree(m_data);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	SetEntry
//
//Last Modified:	March 12, 2004
//
//Purpose:	method to set up a table
//	Sets an state in the state table to respond to a particular message
//
/////////////////////////////////////////////////////////////////////////*/
bool CStateMachine::SetEntry(int state, int message, int newState)
{
	//check if values are valid
	if (state >= m_States || state < 0 || 
		message >= m_Messages || message < 0 || 
		newState >= m_States || newState < 0)
	{
		return false;
	}

	//we have valid values
	m_data[state][message] = newState;
	return true;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	SendStateMessage
//
//Last Modified:	March 12, 2004
//
//Purpose:	method to send a message
//
/////////////////////////////////////////////////////////////////////////*/
bool CStateMachine::SendStateMessage(int msg)
{
	//check if message is within range
	if (msg >= m_Messages || msg < 0)
	{
		return false;
	}

	//we got a valid message
	m_currentState = m_data[m_currentState][msg];
	return true;
}
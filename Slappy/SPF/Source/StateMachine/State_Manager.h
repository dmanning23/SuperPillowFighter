#ifndef _CStateMachine_H_
#define _CStateMachine_H_

/*////////////////////////////////////////////////////////////////////////
//File:	CStateMachine.h
//
//Author:	Dan Manning
//
//Creation Date:	March 12, 2004
//
//Purpose:	Game State machine
//
/////////////////////////////////////////////////////////////////////////*/

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

class CStateMachine
{
private:

	//table of different states and messages
	int **m_data;

	//holds the current state
	int m_currentState;

	//dimensions of array
	int m_States;
	int m_Messages;

	/*////////////////////////////////////////////////////////////////////////
	//Function:	Dummy Assignment Operator
	//
	//Last Modified:	March 12, 2004
	//
	//Input:	CStateMachineRef:	CSM to be copied
	//
	//Output:	NONE
	//
	//Return:	CStateMachine
	//
	//Purpose:	set object equal to another one
	//
	/////////////////////////////////////////////////////////////////////////*/
	CStateMachine &operator=(const CStateMachine &CStateMachineRef);

public:

	/*////////////////////////////////////////////////////////////////////////
	//Function:	Dummy Copy Constructor
	//
	//Last Modified:	March 12, 2004
	//
	//Input:	CStateMachineRef: CSM to be copied
	//
	//Output:	NONE
	//
	//Return:	NONE
	//
	//Purpose:	Copy a CStateMachine into a new one
	//
	/////////////////////////////////////////////////////////////////////////*/
	CStateMachine(const CStateMachine &CStateMachineRef);

	/*////////////////////////////////////////////////////////////////////////
	//Function:	Constructor
	//
	//Last Modified:	March 12, 2004
	//
	//Input:	
	//	numStates:	number of possible states in the machine
	//	numMessages:	number of possible messages in the machine
	//	initialState:	state to start the machine in 
	//
	//Output:	NONE
	//
	//Return:	NONE
	//
	//Purpose:	Create a CState_Machine
	//
	/////////////////////////////////////////////////////////////////////////*/
	CStateMachine(int numStates, int numMessages, int initialState = 0);

	/*////////////////////////////////////////////////////////////////////////
	//Function:	destructor
	//
	//Last Modified:	March 12, 2004
	//
	//Input:	NONE
	//
	//Output:	NONE
	//
	//Return:	NONE
	//
	//Purpose:	Destroy a CStateMachine
	//
	/////////////////////////////////////////////////////////////////////////*/
	virtual ~CStateMachine(void);

	/*////////////////////////////////////////////////////////////////////////
	//Function:	SetEntry
	//
	//Last Modified:	March 12, 2004
	//
	//Input:
	//	state: State to set up a message for
	//	message: message to parse
	//	newState: state this state will change to after getting the message
	//
	//Output:
	//
	//Return:
	//	true if data was valid
	//
	//Purpose:	method to set up a table
	//	Sets an state in the state table to respond to a particular message
	//
	/////////////////////////////////////////////////////////////////////////*/
	bool SetEntry(int state, int message, int newState);

	/*////////////////////////////////////////////////////////////////////////
	//Function:	GetCurrentState
	//
	//Last Modified:	March 12, 2004
	//
	//Input:	NONE
	//
	//Output:	NONE
	//
	//Return:	the integer value of the current state
	//
	//Purpose:	method to get the current state
	//
	/////////////////////////////////////////////////////////////////////////*/
	int GetCurrentState() const { return m_currentState; }

	/*////////////////////////////////////////////////////////////////////////
	//Function:	SendStateMessage
	//
	//Last Modified:	March 12, 2004
	//
	//Input:
	//	msg:  state message to send to machine
	//
	//Output:	NONE
	//
	//Return:	
	//	true if valid message
	//	false if invalid message
	//
	//Purpose:	method to send a message
	//
	/////////////////////////////////////////////////////////////////////////*/
	bool SendStateMessage(int msg);

	/*////////////////////////////////////////////////////////////////////////
	//Function:	ForceState
	//
	//Last Modified:	March 12, 2004
	//
	//Input:	
	//	state:	state to set machine to
	//
	//Output:	NONE
	//
	//Purpose:		Method to force the manager to a certain state
	//	Use only in emergencies!  If you call this function, you'd
	//	better know what you're doing!
	//
	/////////////////////////////////////////////////////////////////////////*/
	void ForceState(int state) { m_currentState = state; }

	/*////////////////////////////////////////////////////////////////////////
	//Function:	GetStates
	//
	//Last Modified:	march 13, 2004
	//
	//Input:	NONE
	//
	//Output:	NONE
	//
	//Return:	m_States
	//
	//Purpose:	provide access to number of states
	//
	/////////////////////////////////////////////////////////////////////////*/
	int GetNumStates() const { return m_States; }

	/*////////////////////////////////////////////////////////////////////////
	//Function:	GetMessages
	//
	//Last Modified:	march 13, 2004
	//
	//Input:	NONE
	//
	//Output:	NONE
	//
	//Return:	m_Messages
	//
	//Purpose:	Provide access to number of messages
	//
	/////////////////////////////////////////////////////////////////////////*/
	int GetNumMessages() const { return m_Messages; }
};

#endif
#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;

struct Pipes
{
	HANDLE h_Childprocess_IN_Rd = NULL;
	HANDLE h_Childprocess_IN_Wr = NULL;
	HANDLE h_Childprocess_OUT_Rd = NULL;
	HANDLE h_Childprocess_OUT_Wr = NULL;
};

//Initialize Cmd
int CmdInitialize(Pipes *p);

// Check current system version
void CheeckSystem();

// Command formatting
string CommandFormat(IN string command);

// Send command to Pipe
int SendCommand_To_Pipe(Pipes *p, IN string command);

// Get result form Pipe
string GetResult_From_Pipe(Pipes *p);

// Run Command
string RunCommand(IN string command);
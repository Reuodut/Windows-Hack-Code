#include "Cmd.h"

//Initialize Cmd
int CmdInitialize(Pipes *p)
{
	SECURITY_ATTRIBUTES saAttr;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&(p->h_Childprocess_OUT_Rd), &(p->h_Childprocess_OUT_Wr), &saAttr, 0))
	{
		printf("Error on CreatePipe!\n");
		return -1;
	}

	if (!SetHandleInformation(p->h_Childprocess_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
	{
		printf("Error on SetHandleInformation!\n");
		return -1;
	}

	if (!CreatePipe(&(p->h_Childprocess_IN_Rd), &(p->h_Childprocess_IN_Wr), &saAttr, 0))
	{
		printf("Error on CreatePipe!\n");
		return -1;
	}

	if (!SetHandleInformation(p->h_Childprocess_IN_Wr, HANDLE_FLAG_INHERIT, 0))
	{
		printf("Error on SetHandleInformation\n");
		return -1;
	}


	PROCESS_INFORMATION piProceInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	ZeroMemory(&piProceInfo, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.wShowWindow = SW_HIDE;
	siStartInfo.hStdError = p->h_Childprocess_OUT_Wr;
	siStartInfo.hStdOutput = p->h_Childprocess_OUT_Wr;
	siStartInfo.hStdInput = p->h_Childprocess_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create child process
	bSuccess = CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProceInfo);
	if (!bSuccess)
	{
#ifdef _DEBUG
		printf("Error on CreateProcess! %d\n", GetLastError());
#endif // _DEBUG
		return -1;
	}
	else
	{
		CloseHandle(piProceInfo.hProcess);
		CloseHandle(piProceInfo.hThread);
	}


	return 0;
}

// Check current system version
void CheeckSystem()
{

}

// Command formatting
string CommandFormat(IN string command)
{
	return "";
}

//Send command to Pipe
int SendCommand_To_Pipe(Pipes *p, IN string command) 
{
	DWORD dwWrite = 0;
	char * command_char = (char *)command.c_str();
	BOOL ret = WriteFile(p->h_Childprocess_IN_Wr, command_char, strlen(command_char), &dwWrite, NULL);
	if (!ret)
	{
		printf("Error on WriteFile!\n");
		return -1;
	}
}

// Get result form Pipe
string GetResult_From_Pipe(Pipes *p)
{
	string result;
	char * buff = (char *)malloc(2048);
	BOOL bSuccess = false;
	DWORD dwRead = 0;
	DWORD dwBytesRead = 0;
	PeekNamedPipe(p->h_Childprocess_OUT_Rd, NULL, 0, NULL, &dwBytesRead, NULL);

	while (true)
	{
		ZeroMemory(buff, 2048);

		if (dwBytesRead <= 0)
		{
			break;
		}

		bSuccess = ReadFile(p->h_Childprocess_OUT_Rd, buff, 2048, &dwRead, NULL);

		if (!bSuccess)
		{
			printf("Error on ReadFile!\n");
			return result;
		}
		result += buff;
		dwBytesRead -= dwRead;
		dwRead = 0;
	}

	free(buff);
	return result;

}

// Close cmd
int CloseCmd(Pipes *p)
{
	return SendCommand_To_Pipe(p, "exit\n");
}

// Run Command
string RunCommand(IN string command)
{
	Pipes *p = (Pipes *)malloc(sizeof(Pipes));
	ZeroMemory(p, sizeof(Pipes));
	
	CmdInitialize(p);

	// Send command to pipe and childprocess
	SendCommand_To_Pipe(p, command + "\n");
	Sleep(1000);

	// Get result form Pipe
	string result;
	while (TRUE)
	{
		result += GetResult_From_Pipe(p);
		char * p = (char*)result.c_str();
		// Command execution completed
		if (p[strlen(p)-1] == '>')
		{
			break;
		}
		Sleep(1000);
	}
	CloseCmd(p);
	return result;

}
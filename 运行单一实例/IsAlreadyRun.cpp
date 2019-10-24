#include <iostream>
#include <Windows.h>

using namespace std;

BOOL IsAleadyRun()
{
	HANDLE hMutex = NULL;
	hMutex = CreateMutex(NULL, FALSE, L"TEST");
	if (hMutex) 
	{ 
		if (ERROR_ALREADY_EXISTS == GetLastError())
		{
			return TRUE;
		}
	}
	return FALSE;
	//不要使用CloseHandle函数来关闭互斥对象的句柄, 关闭后互斥对象会被释放
}

int main ()
{
	BOOL ret = IsAleadyRun();
	
	if (ret)
	{
		cout << "TEST is already run !" << endl;
	}
	else
	{
		cout << "First run of TEST program !" << endl;
	}

	system("pause");
	return 0;
}
// FreeResource.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "resource.h"
#include <windows.h>

// ��ʾ������Ϣ
void FreeRes_ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}

// �ͷ���Դ ����1����Դ�� ����2����Դ���� ����3������·��
BOOL FreeMyResource(UINT uiResouceName, char *lpszResourceType, char *lpszSaveFileName)
{
	// ��ȡָ��ģ�����ָ����Դ
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(uiResouceName), lpszResourceType);
	if (NULL == hRsrc)
	{
		FreeRes_ShowError("FindResource");
		return FALSE;
	}
	// ��ȡ��Դ�Ĵ�С
	DWORD dwSize = ::SizeofResource(NULL, hRsrc);
	if (0 >= dwSize)
	{
		FreeRes_ShowError("SizeofResource");
		return FALSE;
	}
	// ����Դ���ص��ڴ���
	HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		FreeRes_ShowError("LoadResource");
		return FALSE;
	}
	// ������Դ
	LPVOID lpVoid = ::LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		FreeRes_ShowError("LockResource");
		return FALSE;
	}

	// ������ԴΪ�ļ�
	FILE *fp = NULL;
	fopen_s(&fp, lpszSaveFileName, "wb+");
	if (NULL == fp)
	{
		FreeRes_ShowError("LockResource");
		return FALSE;
	}
	fwrite(lpVoid, sizeof(char), dwSize, fp);
	fclose(fp);

	return TRUE;
}

int main()
{
	BOOL ret = FreeMyResource(IDR_DATA2, "DATA", "res.txt");
    return 0;
}


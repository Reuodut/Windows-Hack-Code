#include "RawInput.h"

//����ص�����
LRESULT CALLBACK WinSunProc(
	HWND hwnd,     //handle to window
	UINT uMsg,     //message identifier
	WPARAM wPARAM, //first message parameter
	LPARAM lParam //second message parameter
);

//������ں���
int WINAPI WinMain(
	HINSTANCE hInstance, //handle to current instance
	HINSTANCE hPrevInstance,//handle to previous instance
	LPSTR lpCmdLine,        //command line
	int nCmdShow            //show state
)
{
	//���һ��������
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndcls.hInstance = hInstance;    //Ӧ�ó���ʵ�������WinMain����������
	wndcls.lpfnWndProc = WinSunProc; // ����ص�������ַ
	wndcls.lpszClassName = "mywindow";
	wndcls.lpszMenuName = NULL;
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndcls);        //ע�ᴰ����
								   //�������ڣ�����һ��������������ɹ��������ں󷵻صľ��
	HWND hwnd;
	hwnd = CreateWindow("mywindow", "my first application",
		WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, hInstance, NULL);
	//��ʾ��ˢ�´���,SW_HIDE ����Ϊ��������
	ShowWindow(hwnd, SW_HIDE);
	UpdateWindow(hwnd);
	// ע��ԭʼ�����豸
	Init(hwnd);
	//������Ϣ�ṹ�壬��ʼ��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); //���������Ϣת��Ϊ�ַ���Ϣ
		DispatchMessage(&msg); //����Ϣ���ݸ�����ϵͳ
	}
	return msg.wParam;
}
//��д���ڹ��̺���
LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		Init(hwnd);
		break;
	case WM_INPUT:
		GetData(lParam);
		break;
	case WM_CHAR:           //�ڴ����а���һ���ַ���
		break;
	case WM_LBUTTONDOWN:    //�ڴ����а���������ʱ
		break;
	case WM_PAINT:         //���»��ƴ���
		break;
	case WM_CLOSE:          //�رմ���ʱ������Ϣ��
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

